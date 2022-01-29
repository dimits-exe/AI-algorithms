from load_imdb import load_examples, load_attributes, Example
from id3 import Category, ID3
from random_forest import RandomForest
from test_stats import TestStats
from classifier import Classifier
from timed import timed

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


@timed(prompt= "Main program")
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
        print(data_dir)
        train_data_dir = os.path.join(data_dir, "train")
        print(train_data_dir)
        test_data_dir = os.path.join(data_dir, "test")
        vocab_file_name = os.path.join(data_dir, "imdb.vocab")

        example_size = check_int_arg(sys.argv[2], "example size", 100, 250000)
        ignore_attr_count = check_int_arg(sys.argv[3], "ignored words count", 0, 90000)
        attr_count = check_int_arg(sys.argv[4], "total word count", 5, 90000)

        train_data = load_examples(train_data_dir, example_size)
        testing_data = load_examples(test_data_dir, example_size)
        attributes = load_attributes(vocab_file_name, attr_count, ignore_attr_count)

        id3 = ID3.create_timed(train_data, attributes)
        print("ID3 training results: ", test_classifier(id3, train_data))
        print("ID3 testing results: ",  test_classifier(id3, testing_data))

        rand_forest = RandomForest(train_data, attributes, 0.1)
        print("Random Forest training results: ", test_classifier(rand_forest, train_data))
        print("Random Forest testing results: ", test_classifier(rand_forest, testing_data))


if __name__ == "__main__":
    try:
        main()
    except IOError as ioe:
        print("Error: ", ioe)
        print("Please make sure the first argument is the upper-most directory ('aclImdb') containing the imdb dataset")
