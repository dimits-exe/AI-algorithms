import os

import load_imdb

from classifier_evaluation import ClassifierEvaluation
from id3 import ID3
from random_forest import RandomForest


def main():
    directory = "C:\\Users\\alexm\\projects\\C++\\AI-algorithms\\resources\\aclImdb\\"
    train = "train"
    test = "test"
    sample_size = 10

    train_examples = load_imdb.load_examples(os.path.join(directory, train), sample_size)
    attributes = load_imdb.load_attributes(train_examples)

    train_examples2 = {e.copy() for e in train_examples}

    id3 = ID3(train_examples, attributes)
    random_forest = RandomForest(train_examples2, attributes, tree_count=3, percent_examples_per_tree=0.1)

    test_examples = load_imdb.load_examples(os.path.join(directory, test), sample_size)
    test_examples2 = {e.copy() for e in test_examples}

    id3.classify_bulk(test_examples)
    random_forest.classify_bulk(test_examples2)

    id3_stats = ClassifierEvaluation(test_examples)
    rf_stats = ClassifierEvaluation(test_examples2)


if __name__ == "__main__":
    main()
