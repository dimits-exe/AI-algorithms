import os

from classifier import Example, Category
from timed import timed


@timed(prompt="Load Data")
def load_examples(directory: str, sample_size=5000) -> set[Example]:
    neg_dir_path = os.path.join(directory, "neg")
    pos_dir_path = os.path.join(directory, "pos")

    examples = set()
    examples |= _load_examples_of_category(neg_dir_path, Category.POS, sample_size // 2)
    examples |= _load_examples_of_category(pos_dir_path, Category.NEG, sample_size // 2)

    return examples


def _load_examples_of_category(directory: str, category: Category, sample_size: int) -> set[Example]:
    print(f"Loading {category.name} data")
    files = os.listdir(directory)
    sep = os.sep
    examples = set()
    one_tenth_progress = min(len(files), sample_size) / 10

    for count, file in enumerate(files):
        if count == sample_size:
            break

        with open(directory + sep + file, mode='r', encoding="utf8") as f_handle:
            contents = f_handle.read().lower()

        examples.add(Example(category, contents))

        if (count + 1) % one_tenth_progress == 0:  # `count` is 0-indexed
            print('\b' * 20 + "%d%% complete..." % ((count + 1) // one_tenth_progress * 10), end="")

    print()

    return examples


@timed(prompt="Extract Attributes")
def load_attributes(examples: set[Example]) -> set[str]:
    # TODO replace load_attributes(filename: str) that reads the file with the attributes
    attributes = set()
    for example in examples:
        attributes |= example.attributes

    return attributes


@timed(prompt="Test Load")
def test_load() -> None:

    # path_to_file = os.path.dirname(os.path.abspath(__file__))
    path_to_file = "C:\\Users\\alexm\\projects\\C++\\AI-algorithms\\resources"
    directory = os.path.join(path_to_file, "aclImdb", "train")

    try:
        examples = load_examples(directory, 10000)
        attributes = load_attributes(examples)

    except os.error as err:
        print(f"Loading didn't complete normally due to: {err}")
        return


if __name__ == "__main__":
    test_load()
