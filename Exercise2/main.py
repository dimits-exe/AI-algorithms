from load_imdb import load_examples, get_attributes, Example
from id3 import Category, ID3
from random_forest import RandomForest
from test_stats import TestStats
import sys


def test_classifier(classifier: object, examples: set[Example]) -> TestStats:
    """
    Run a test on a given classifier for a given dataset.
    :param classifier: any object that implements the `classify(set[Example])` method
    :param examples: the examples to be tested
    :return: a TestStats object describing the results of the testing
    """

    false_positives = 0
    false_negatives = 0
    true_positives = 0
    for example in examples:
        classifier.classify(example)
        if example.actual == Category.POS and example.predicted == Category.POS:
            true_positives += 1
        elif example.actual == Category.POS and example.predicted == Category.NEG:
            false_negatives += 1
        elif example.actual == Category.NEG and example.predicted == Category.POS:
            false_positives += 1

    return TestStats(len(examples), true_positives, false_positives, false_negatives)


def test_id3(train_data_directory: str, test_data_directory: str, example_size: int,
             ignore_attribute_count: int, attribute_count: int) -> tuple[TestStats, TestStats]:
    """
    Trains and tests the ID3 algorithm on both the training and testing data and returns
    the results of the tests.
    :param train_data_directory: the directory in which the files containing the training data are located
    :param test_data_directory: the directory in which the files containing the testing data are located
    :param example_size: the number of examples to be loaded for the training and testing
    :param ignore_attribute_count: how many of the most common tokens will be ignored
    :param attribute_count: the total amount of tokens used for classification
    :return: a tuple with the TestStats of the ID3 tree on the training and testing data
    """
    train_data = load_examples(train_data_directory, example_size)
    testing_data = load_examples(test_data_directory, example_size)

    attributes = get_attributes(train_data)

    id3 = ID3(train_data, attributes)

    train_id3_results = test_classifier(id3, train_data)
    test_id3_results = test_classifier(id3, testing_data)

    return train_id3_results, test_id3_results


def test_rand_forest(train_data_directory: str, test_data_directory: str, example_size: int,
                     ignore_attribute_count: int, attribute_count: int, trees: int) -> tuple[TestStats, TestStats]:
    """
    Trains and tests the Random Forest algorithm on both the training and testing data and returns
    the results of the tests.
    :param train_data_directory: the directory in which the files containing the training data are located
    :param test_data_directory: the directory in which the files containing the testing data are located
    :param example_size: the number of examples to be loaded for the training and testing
    :param ignore_attribute_count: how many of the most common tokens will be ignored
    :param attribute_count: the total amount of tokens used for classification
    :param trees: the amount of trees used in the Random Forest classifier
    :return: a tuple with the TestStats of the Random Forest classifier on the training and testing data
    """
    train_data = load_examples(train_data_directory, example_size)
    testing_data = load_examples(test_data_directory, example_size)
    attributes = get_attributes(train_data)

    rand_forest = RandomForest(train_data, attributes, trees, 0.33)

    train_rand_forest_results = test_classifier(rand_forest, train_data)
    test_rand_forest_results = test_classifier(rand_forest, testing_data)

    return train_rand_forest_results, test_rand_forest_results


def main() -> None:
    def check_int_arg(arg: str, bottom_limit: int, upper_limit: int) -> int:
        try:
            answer = int(arg)
        except ValueError:
            answer = -1

        if answer < bottom_limit or answer > upper_limit:
            print(f"Error: parameter must be a valid integer within the [{bottom_limit}, {upper_limit}] range")
            sys.exit(1)
        else:
            return answer

    if len(sys.argv) < 7:
        print("Insufficient parameters:")
        print("Use main.py <training data directory> <testing data directory> <number of examples to be loaded>"
              " <number of ignored words> <number of words to be considered> <number of trees>")
    else:
        train_data_dir = sys.argv[1]
        test_data_dir = sys.argv[2]

        example_size = check_int_arg(sys.argv[3], 100, 250000)
        ignore_attr_count = check_int_arg(sys.argv[4], 0, 90000)
        attr_count = check_int_arg(sys.argv[5], 5, 90000)
        tree_count = check_int_arg(sys.argv[6], 1, 250)

        id3_results = test_id3(train_data_dir, test_data_dir, example_size, ignore_attr_count, attr_count)
        print("ID3 training results: ", id3_results[0])
        print("ID3 testing results: ", id3_results[1])

        forest_results = test_rand_forest(train_data_dir, test_data_dir, example_size, ignore_attr_count,
                                          attr_count, tree_count)
        print("Random Forest training results: ", forest_results[0])
        print("Random Forest testing results: ", forest_results[1])


if __name__ == "__main__":
    main()
