import os
import sys

from classifier_evaluation import ClassifierEvaluation
import load_imdb

from classifier import Classifier, Example, Category
from id3 import ID3
from random_forest import RandomForest

from test_stats import TestStats


def test_classifier(classifier: Classifier, examples: set[Example]) -> TestStats:
    """
    Run a test on a given classifier for a given dataset.

    :param classifier: any Classifier object
    :param examples: the examples to be tested
    :return: a TestStats object describing the results of the testing
    """

    false_positives = 0
    false_negatives = 0
    true_positives = 0

    classifier.classify_bulk(examples)

    for example in examples:
        if example.actual == Category.POS and example.predicted == Category.POS:
            true_positives += 1
        elif example.actual == Category.POS and example.predicted == Category.NEG:
            false_negatives += 1
        elif example.actual == Category.NEG and example.predicted == Category.POS:
            false_positives += 1

    return TestStats(len(examples), true_positives, false_positives, false_negatives)


def _load_examples(train_dir: str, test_dir: str, example_size: int) -> tuple[set[Example], set[Example]]:
    """
    Loads training and testing examples from the given directories.

    :param train_dir: the directory in which the files containing the training data are located
    :param test_dir: the directory in which the files containing the testing data are located
    :param example_size: the total number of examples to be loaded
    :return: a tuple with a set of training and a set of testing examples
    """

    training_data = load_imdb.load_examples(train_dir, example_size)
    testing_data = load_imdb.load_examples(test_dir, example_size)

    return training_data, testing_data


def _load_attributes(attr_file: str, attribute_count: int, ignore_attribute_count: int) -> set[str]:
    """
    Loads attributes from the given file.

    :param attr_file: the file containing the attributes sorted by frequency
    :param attribute_count: the total amount of attributes used for classification
    :param ignore_attribute_count: how many of the most common attributes will be ignored
    :return: the attributes read
    """
    return load_imdb.load_attributes(attr_file, attribute_count, ignore_attribute_count)


def test_id3(training_data: set[Example], testing_data: set[Example], attributes: set[str], cutoff: float) -> \
        tuple[TestStats, TestStats]:
    """
    Trains a new ID3 classifier on the training data and tests it both on the training and testing data
    and returns the results of the tests.

    :param training_data: the set of examples with which the classifier will be trained and tested
    :param testing_data: the set of examples with which the classifier will be tested
    :param attributes: the attributes that will be used to classify the examples
    :param cutoff: the cutoff point of the ID3 classifier
    :return: a tuple with the TestStats of the ID3 tree on the training and testing data
    """

    id3 = ID3(training_data, attributes, cutoff)

    train_id3_results = test_classifier(id3, training_data)
    test_id3_results = test_classifier(id3, testing_data)

    return train_id3_results, test_id3_results


def test_rand_forest(training_data: set[Example], testing_data: set[Example], attributes: set[str], cutoff: float,
                     tree_count: int, perc_attrs: float) -> tuple[TestStats, TestStats]:
    """
    Trains and tests the Random Forest algorithm on both the training and testing data and returns
    the results of the tests.

    :param training_data: the set of examples with which the classifier will be trained and tested
    :param testing_data: the set of examples with which the classifier will be tested
    :param attributes: the attributes that will be used to classify the examples
    :param cutoff: the cutoff point of the ID3 classifier
    :param tree_count: the amount of trees used in the Random Forest classifier
    :param perc_attrs: the percentage of attributes that each tree will consider.
    :return: a tuple with the TestStats of the Random Forest classifier on the training and testing data
    """
    rand_forest = RandomForest(training_data, attributes, tree_count, perc_attrs, cutoff)

    train_rand_forest_results = test_classifier(rand_forest, training_data)
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

    # TODO: fix this lmao, arguments have changed
    if len(sys.argv) < 10:
        print("Insufficient parameters:")
        print(
            "\nUse main.py <training data directory> <testing data directory> <number of examples to be loaded>"
            " <number of ignored words> <number of words to be considered> <number of trees>")
    else:
        train_data_dir = sys.argv[1]
        test_data_dir = sys.argv[2]
        example_size = check_int_arg(sys.argv[3], 100, 250000)

        attr_filename = sys.argv[4]
        attr_count = check_int_arg(sys.argv[6], 5, 90000)
        ignore_attr_count = check_int_arg(sys.argv[5], 0, 90000)

        cutoff = float(sys.argv[8])
        tree_count = check_int_arg(sys.argv[7], 1, 250)
        perc_attrs = float(sys.argv[9])

        training_data, testing_data = _load_examples(train_data_dir, test_data_dir, example_size)
        attributes = _load_attributes(attr_filename, attr_count, ignore_attr_count)

        id3_results = test_id3(training_data, testing_data, attributes, cutoff)
        print("ID3 training results: ", id3_results[0])
        print("ID3 testing results: ", id3_results[1])

        forest_results = test_rand_forest(training_data, testing_data, attributes, cutoff,
                                          tree_count, perc_attrs)
        print("Random Forest training results: ", forest_results[0])
        print("Random Forest testing results: ", forest_results[1])


def hardcoded_main():
    path_to_imdb = "C:\\Users\\alexm\\projects\\C++\\AI-algorithms\\resources\\aclImdb"
    train_data_dir = os.path.join(path_to_imdb, "train")
    test_data_dir = os.path.join(path_to_imdb, "test")
    example_size = 5000
    attr_filename = os.path.join(path_to_imdb, "imdb.vocab")
    attr_count = 1000
    ignore_attr_count = 0
    cutoff = 0.95
    tree_count = 1
    perc_attrs = 1

    training_data, testing_data = _load_examples(train_data_dir, test_data_dir, example_size)
    attributes = _load_attributes(attr_filename, attr_count, ignore_attr_count)

    id3_results = test_id3(training_data, testing_data, attributes, cutoff)
    print("ID3 training results: ", id3_results[0])
    print("ID3 testing results: ", id3_results[1])
    print("ID3 better training results: ", id3_results[2])
    print("ID3 better testing results: ", id3_results[3])

    forest_results = test_rand_forest(training_data, testing_data, attributes, cutoff,
                                      tree_count, perc_attrs)
    print("Random Forest training results: ", forest_results[0])
    print("Random Forest testing results: ", forest_results[1])


if __name__ == "__main__":
    # main()
    hardcoded_main()
