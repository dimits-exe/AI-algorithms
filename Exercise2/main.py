from load_imdb import load_examples, load_attributes, Example
from id3 import Category, ID3
from random_forest import RandomForest
from test_stats import TestStats
from classifier import Classifier
from timed import timed

from multiprocessing import Process
import os
import sys


def test_classifier(classifier: Classifier, examples: set[Example]) -> TestStats:
    """
    Run a test on a given classifier for a given dataset.
    :param classifier: any Classifier object
    :param examples: the dataset to be tested
    :return: a TestStats object describing the results of the testing
    """
    false_positives = 0
    false_negatives = 0
    true_positives = 0
    true_negatives = 0

    classifier.classify_bulk(examples)

    for example in examples:
        if example.actual == Category.POS and example.predicted == Category.POS:
            true_positives += 1
        elif example.actual == Category.NEG and example.predicted == Category.NEG:
            true_negatives += 1
        elif example.actual == Category.POS and example.predicted == Category.NEG:
            false_negatives += 1
        elif example.actual == Category.NEG and example.predicted == Category.POS:
            false_positives += 1

    return TestStats(true_negatives, true_positives, false_positives, false_negatives)


def test_id3(train_data, testing_data, attributes):
    id3 = ID3.create_timed(train_data, attributes)
    test_train_id3_process = Process(target=test_and_print, args=("ID3 training results: ", id3, train_data))
    test_test_id3_process = Process(target=test_and_print, args=("ID3 testing results: ", id3, testing_data))

    test_train_id3_process.start()
    test_test_id3_process.start()

    test_train_id3_process.join()
    test_test_id3_process.join()


def test_rand_forest(train_data, testing_data, attributes):
    rand_forest = RandomForest(train_data, attributes, 0.1)

    test_train_rand_forest_process = Process(target=test_and_print,
                                             args=("Random Forest training results: ", rand_forest, train_data))
    test_test_rand_forest_process = Process(target=test_and_print,
                                            args=("Random Forest testing results: ", rand_forest, testing_data))
    test_train_rand_forest_process.start()
    test_test_rand_forest_process.start()

    test_train_rand_forest_process.join()
    test_test_rand_forest_process.join()


def test_and_print(prompt: str, classifier: Classifier, examples: set[Example]):
    print(prompt, test_classifier(classifier, examples))


def main_test(train_data_dir: str, test_data_dir: str, vocab_file_dir: str,
              example_size: int, attr_count: int, ignore_attr_count: int) -> None:

    train_data = load_examples(train_data_dir, example_size)
    testing_data = load_examples(test_data_dir, example_size)
    attributes = load_attributes(vocab_file_dir, attr_count, ignore_attr_count)

    create_id3_process = Process(target=test_id3, args=(train_data, testing_data, attributes))
    create_rand_forest_process = Process(target=test_rand_forest, args=(train_data, testing_data, attributes))

    create_rand_forest_process.start()
    create_id3_process.start()
    create_rand_forest_process.join()
    create_id3_process.join()


@timed(prompt="Main program")
def main() -> None:
    def check_int_arg(arg: str, param_name: str, bottom_limit: int, upper_limit: int) -> int:
        try:
            answer = int(arg)
        except ValueError:
            answer = -1

        if answer < bottom_limit or answer > upper_limit:
            print(f"Error: parameter `{param_name}` must be a valid integer within the "
                  f"[{bottom_limit}, {upper_limit}] range")
            sys.exit(1)
        else:
            return answer

    if len(sys.argv) < 5:
        print("Insufficient parameters:")
        print("Use main.py <imdb directory> <number of examples to be loaded>"
              " <number of ignored words> <number of words to be considered>")
    else:
        data_dir = sys.argv[1]
        train_data_dir = os.path.join(data_dir, "train")
        test_data_dir = os.path.join(data_dir, "test")
        vocab_file_name = os.path.join(data_dir, "imdb.vocab")

        example_size = check_int_arg(sys.argv[2], "example size", 100, 250000)
        ignore_attr_count = check_int_arg(sys.argv[3], "ignored words count", 0, 90000)
        attr_count = check_int_arg(sys.argv[4], "total word count", 5, 90000)

        main_test(train_data_dir, test_data_dir, vocab_file_name, example_size, attr_count, ignore_attr_count)


if __name__ == "__main__":
    try:
        main()
    except IOError as ioe:
        print("Error: ", ioe)
        print("Please make sure the first argument is the upper-most directory ('aclImdb') containing the imdb dataset")
