import random
import math

from id3 import Example, Category, ID3_Tree


class RandomForest:
    """
    A Random Forest classifier internally using ID3 trees to classify examples.
    """

    def __init__(self, examples: set[Example], attributes: set[str], tree_count: int, percent_examples_per_tree: float):
        """
        Creates a new Random Forest classifier that uses a number of trees, each trained on a percentage
        of the examples, to classify an Example. Note that training 5 trees each on 100% of the data or
        10 trees each on 1% of the data are both valid combinations; the product of number-of-trees times
        percentage-of-examples need not be 100%.

        :param examples: the examples on which to train the Random Forest classifier
        :param attributes: the attributes that will be used to classify the examples
        :param tree_count: the number of trees used in the classifier
        :param percent_examples_per_tree: the percentage of Examples that each tree will train on.
        """
        examples = tuple(examples)
        example_count_per_tree = math.floor(len(examples) * percent_examples_per_tree)

        self.trees: set[ID3_Tree] = set()

        for _ in range(tree_count):
            # pass copies of the examples, so they properly hold their "predicted" value
            examples_for_tree = {e.copy() for e in random.sample(examples, k=example_count_per_tree)}
            trained_tree = ID3_Tree.train(set(examples_for_tree), attributes)
            self.trees.add(trained_tree)

    def classify(self, example: Example) -> Category:
        """
        Classifies the provided example by plurality vote of the trees created during initialization.

        :param example: The example to be classified
        :return: The predicted category of the example
        """
        category_count: dict[Category, int] = dict.fromkeys(Category.values(), 0)

        for tree in self.trees:
            category_count[tree.classify(example)] += 1

        return max(category_count.keys(), key=lambda k: category_count[k])
