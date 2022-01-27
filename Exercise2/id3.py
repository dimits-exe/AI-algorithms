import math

from classifier import Example, Category, Classifier
from timed import timed


class Node:
    """
    An internal data structure used to construct an ID3 tree and later traverse it.
    The `attribute` field indicates the attribute that is being tested at this Node and the
    `children` dictionary contains references to the appropriate Nodes that should be traversed
    based on the value of the `attribute` in an Example.
    The `category` field is NONE iff this Node is internal in the tree. A non-NONE value indicates
    that upon reaching this Node, the predicted Category of an Example will be that category.
    """

    def __init__(self, category: Category, attribute: str):
        self.category: Category = category
        self.children: dict[bool, Node] = dict()
        self.attribute: str = attribute

    @staticmethod
    def internal(attribute: str) -> 'Node':
        """
        Returns an internal Node of an ID3 tree, responsible for an attribute.
        The Node's Category is set to NONE.
        :param attribute: the attribute that shall be checked in this Node
        :return: the Node
        """
        return Node(Category.NONE, attribute)

    @staticmethod
    def leaf(category: Category) -> 'Node':
        """
        Returns a leaf Node of an ID3 tree, responsible for a classification.
        The Node's Attribute is set to "".
        :param category: the Category with which Examples will be classified according to this Node
        :return: the Node
        """
        return Node(category, "")


class ID3(Classifier):
    """ An ID3 Tree classifier used to classify an Example """

    # a parameter to be supplied as the cutoff point in the id3_recursive function
    pruning_parameter = 1

    @timed(prompt="Train ID3")
    def __init__(self, examples: set[Example], attributes: set[str]):
        """
        Creates a new ID3 classifier by training it on the provided training data.
        :param examples: the examples on which to train the ID3 classifier
        :param attributes: the attributes that will be used to classify the examples
        """
        self.root: Node = ID3.id3_recursive(examples, attributes, Category.NONE, ID3.pruning_parameter)

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

    @staticmethod
    def id3_recursive(examples: set[Example], attributes: set[str], target_category: Category,
                      cutoff_point: float) -> Node:
        """
        Generates a tree that can classify an example.
        :param examples: the set of examples from which the tree will be constructed
        :param attributes: the attributes that will be used to classify the examples
        :param target_category: the most common category among the examples
        :param cutoff_point: if cutoff_point% training examples or more
        belong in one of the leaf nodes, the algorithm will cease expanding the tree
        :return: a tree node that best classifies the examples with the given attributes
        """
        # if there are no examples, return target_category
        if len(examples) == 0:
            return Node.leaf(target_category)

        # if all examples belong to a single category, return that category
        for category in Category.values():
            if all(e.actual == category for e in examples):
                return Node.leaf(category)

        # find most common category among all the examples
        categories = dict.fromkeys(Category.values(), 0)
        for example in examples:
            categories[example.actual] += 1
        most_common_category = max(categories.keys(), key=lambda k: categories[k])

        # if there are no attributes left, return the most common category
        if len(attributes) == 0:
            return Node.leaf(most_common_category)

        # otherwise, create a tree by splitting the examples by whether
        # they contain best_attr or not (values True or False)
        best_attr = choose_best_attr(attributes, examples)
        root = Node.internal(best_attr)

        for value in {True, False}:
            examples_subset = {e for e in examples if (best_attr in e.attributes) == value}
            new_attrs = set(attributes)
            new_attrs.remove(best_attr)

            # if sufficient categorization, end the tree expansion early
            if len(examples_subset) / len(examples) > cutoff_point:
                return Node.leaf(most_common_category)
            else:
                subtree = ID3.id3_recursive(examples_subset, new_attrs, most_common_category, cutoff_point)
                root.children[value] = subtree

            #subtree = ID3.id3_recursive(examples_subset, new_attrs, most_common_category, cutoff_point)
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
    else:
        return -probability * math.log2(probability) - (1 - probability) * math.log2(1 - probability)


def test_id3() -> None:
    e = set()
    p = Category.POS
    n = Category.NEG
    e.add(Example(p, "A B C"))
    e.add(Example(p, "A B C"))
    e.add(Example(p, "A C"))
    e.add(Example(n, "A B"))
    e.add(Example(n, "B C"))
    e.add(Example(n, "C"))

    tree = ID3(e, {"A", "B", "C"})


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
    test_id3()
