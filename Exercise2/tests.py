import os

from timed import timed

from load_imdb import load_examples, load_attributes

from id3 import entropy


@timed(prompt="Test Load")
def test_load(sample_size: int, count: int, ignore: int) -> None:

    # path_to_file = os.path.dirname(os.path.abspath(__file__))
    path_to_imdb = "C:\\Users\\alexm\\projects\\C++\\AI-algorithms\\resources\\aclImdb"
    example_dir = os.path.join(path_to_imdb, "train")
    attribute_dir = os.path.join(path_to_imdb, "imdb.vocab")

    try:
        examples = load_examples(example_dir, sample_size)
        attributes = load_attributes(attribute_dir, count, ignore)

    except os.error as err:
        print(f"Loading didn't complete normally due to: {err}")
        return

    for example in examples:
        print(example)

    print(attributes, sep="\n")


def test_entropy(*probabilities: float) -> list[float]:
    """
    Prints and returns the entropy for every probability provided and runs some hard-coded assertions

    :param probabilities: the probabilities to calculate the entropy for
    :return: a list that has the entropy of probability i at index i
    """
    entropies = [entropy(p) for p in probabilities]
    for probability, probability_entropy in zip(probabilities, entropies):
        print("H(%0.2f) = %0.5f" % (probability, probability_entropy))

    assert entropy(1) == 0
    assert entropy(0) == 0
    assert entropy(1/2) == 1

    return entropies


if __name__ == '__main__':
    # test_load(5, 3, 10)
    test_entropy()
