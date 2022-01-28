import os
from timed import timed

from load_imdb import load_examples, load_attributes


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


if __name__ == '__main__':
    test_load(5, 3, 10)
