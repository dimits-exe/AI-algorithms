from math import log2
import re

from enum import Enum, auto
from timed import timed


class Category(Enum):
    NONE = auto()
    POS = auto()
    NEG = auto()

    @classmethod
    def values(cls):
        return {cls.POS, cls.NEG}


class Example:
    _ignored_chars = ['"', "'", '.', ',', '>', '<', '\\', '/', '-', '(', ')', ';', ':', '?']
    _regex = "[%s\\d]" % (re.escape("".join(_ignored_chars)))
    _ignored_chars_pattern = re.compile(_regex)

    def __init__(self, category: Category, raw_text: str):
        self.actual = category
        self.predicted = Category.NONE

        sanitized_text = Example._ignored_chars_pattern.sub("", raw_text, 0)
        sanitized_text = re.sub("\\s+", " ", sanitized_text)
        self.attributes = set(sanitized_text.split(" "))

    def __str__(self):
        return f"{self.actual.name}: {self.attributes}"


class Node:
    """
    An internal data structure used to construct a ID3 tree.
    """

    def __init__(self, category: Category, attribute: str):
        self.category = category
        self.children = dict()
        self.attribute = attribute

class ID3_Tree:
    """ An ID3 Tree classifier used to classify an Example """
    
    @timed(prompt="Run ID3")
    def __init__(self, examples: set[Example], attributes: set[str]):
        """
        Creates a new ID3 classifier by training it on the provided training data.

        :param examples: the examples on which to train the ID3 classifier
        :param attributes: the attributes that will be used to classify the examples
        """
        self.root: Node = id3_recursive(examples, attributes, Category.NONE)

    def classify(self, test_example: Example) -> Category:
        """
        Classifies the provided Example by traversing the internal tree based on the
        Example's attributes.The `predicted` Category of the test_example is also updated

        :param test_example: The example to be classified
        :return: The predicted Category of the example.
        """
        curr: Node = self.root
        while curr.category == Category.NONE:
            curr = curr.children[curr.attribute in test_example.attributes]

        test_example.predicted = curr.category
        return curr.category


def id3_recursive(examples: set[Example], attributes: set[str], target_category: Category) -> Node:
    """
    Generates a tree that can classify an example.

    :param examples: the set of examples from which the tree will be constructed
    :param attributes: the attributes that will be used to classify the examples
    :param target_category: the most common category among the examples
    :return: a tree node that best classifies the examples with the given attributes
    """
    # if there are no examples, return target_category
    if len(examples) == 0:
        return Node(target_category, "")

    # if all examples belong to a single category, return that category
    for category in Category.values():
        if all(e.actual == category for e in examples):
            return Node(category, "")

    # find most common category among all the examples
    categories = dict.fromkeys(Category.values(), 0)
    for example in examples:
        categories[example.actual] += 1
    most_common_category = max(categories.keys(), key=lambda k: categories[k])

    # if there are no attributes left, return the most common category
    if len(attributes) == 0:
        return Node(most_common_category, "")

    # otherwise, create a tree by splitting the examples by whether
    # they contain best_attr or not (values True or False)
    best_attr = choose_best_attr(attributes, examples)
    root = Node(Category.NONE, best_attr)

    for value in {True, False}:
        examples_subset = {e for e in examples if (best_attr in e.attributes) == value}
        new_attrs = set(attributes)
        new_attrs.remove(best_attr)
        subtree = id3_recursive(examples_subset, new_attrs, most_common_category)
        root.children[value] = subtree

    return root


def choose_best_attr(attributes: set[str], examples: set[Example]) -> str:
    """
    Returns the attribute with the maximum information gain calculated for a set of Examples.

    :param attributes: the attributes to be examined for information gain
    :param examples: the examples for which the information gain will be calculated
    :return: the attribute with the maximum information gain for the examples given
    """
    # find the ratio of all positive reviews
    positive_reviews = negative_reviews = 0

    for example in examples:
        if example.actual == Category.POS:
            positive_reviews += 1
        elif example.actual == Category.NEG:
            negative_reviews += 1
        else:
            print(f"error: example {example} has category {example.actual}")

    ratio = positive_reviews / (positive_reviews + negative_reviews)

    # find the word with the maximum information gain
    max_gain = -1
    max_gain_word = ""
    for attribute in attributes:
        gain = info_gain(ratio, examples, attribute)
        if gain > max_gain:
            max_gain = gain
            max_gain_word = attribute

    return max_gain_word


def info_gain(positive_review_ratio: float, examples: set[Example], word: str) -> float:
    """
    Returns the information gain of a word in a set of Examples.
    
    :param positive_review_ratio: the ratio of positive to all reviews
    :param examples: the set of examples among which to calculate the information gain
    :param word: the word for which to calculate the information gain
    :return: the information gain of that word in the examples
    """

    pos_examples = neg_examples = 0
    word_count_in_examples = 0

    for example in examples:
        if word in example.attributes:
            word_count_in_examples += 1
            if example.actual == Category.POS:
                pos_examples += 1
            elif example.actual == Category.NEG:
                neg_examples += 1
            else:
                print(f"error: example {example} has category {example.actual}")

    if word_count_in_examples == 0:
        return 0.0

    pos_chance = pos_examples / (pos_examples + neg_examples)
    percentage_examples_with_word = word_count_in_examples / len(examples)

    # H(C) - P(X=1) * H(C|X=1) + P(X=0) * H(C|X=0)
    return entropy(positive_review_ratio) - percentage_examples_with_word * entropy(pos_chance) \
        + (1 - percentage_examples_with_word) * entropy(1 - pos_chance)


def entropy(probability: float) -> float:
    """
    Returns the entropy associated with the probability of an event.

    :param probability: the probability of the event
    :return: the entropy of that event
    """

    if probability == 0.0 or probability == 1.0:
        return 0.0

    return -probability * log2(probability) - (1 - probability) * log2(1 - probability)


def id3_test() -> None:
    e = set()
    p = Category.POS
    n = Category.NEG
    e.add(Example(p, "A B C"))
    e.add(Example(p, "A B C"))
    e.add(Example(p, "A C"))
    e.add(Example(n, "A B"))
    e.add(Example(n, "B C"))
    e.add(Example(n, "C"))

    tree = ID3_Tree(e, {"A", "B", "C"})
    _preorder(tree.root, 0)


def _preorder(node: Node, depth: int) -> None:
    print(depth, end=" ")
    if node.category != Category.NONE:
        print(node.category)

    else:
        print(node.attribute)
        for attribute, child in node.children.items():
            _preorder(child, depth + 1)


def choose_best_attr_test() -> None:
    r1 = Example(Category.NEG, "owo uwu owo angry owo")
    r2 = Example(Category.POS, "very normal words yes")
    r3 = Example(Category.NEG, "again angry owo and not so angry uwu")
    r4 = Example(Category.POS, "very cool review yes yes")

    reviews = set()
    reviews.add(r1)
    reviews.add(r2)
    reviews.add(r3)
    reviews.add(r4)
    words = {"owo", "yes", "again"}

    print(choose_best_attr(words, reviews))


if __name__ == "__main__":
    id3_test()
