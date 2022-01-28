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


@timed(prompt="Load Attributes")
def load_attributes(filename: str, most_frequent_count: int, ignored_count: int) -> tuple[str]:
    """
    Loads the m most frequent attributes from a file, ignoring the first n.
    The attributes in the file are sorted by frequency in descending order

    :param filename: the file containing the attributes
    :param most_frequent_count: the number of most frequent attributes to read
    :param ignored_count: the number of most frequent attributes to ignore
    """

    attributes = list()

    with open(filename, mode='r', encoding="utf8") as f:
        for _ in range(ignored_count):
            f.readline()
        for _ in range(most_frequent_count):
            attribute = f.readline().strip('\n')
            attributes.append(Example.sanitize_attribute(attribute))

    return tuple(attributes)


def _load_examples_of_category(directory: str, category: Category, sample_size: int) -> set[Example]:
    print(f"Loading {category.name} data")
    files = os.listdir(directory)
    sep = os.sep
    examples = set()
    one_tenth_progress = min(len(files), sample_size) // 10

    for count, file in enumerate(files):
        if count == sample_size:
            break

        with open(directory + sep + file, mode='r', encoding="utf8") as f_handle:
            contents = f_handle.read().lower()

        examples.add(Example(category, contents))

        if one_tenth_progress != 0 and (count + 1) % one_tenth_progress == 0:  # `count` is 0-indexed
            print('\b' * 20 + "%d%% complete..." % ((count + 1) // one_tenth_progress * 10), end="")

    print()

    return examples
