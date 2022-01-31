import os
import sys
from main import main_test


def get_overfit_data(data_dir: str) -> tuple[list[float], list[float], list[float], list[float]]:
    """
    Get the accuracy of repeated executions of the ID3 and Random Forest classifiers with
    variable parameters.
    :param data_dir: the directory containing the training examples
    :return: A tuple containing 4 lists of floats. More specifically:
    0: A list with the accuracy of the ID3 tree using the training data
    1: A list with the accuracy of the ID3 tree using the testing data
    2: A list with the accuracy of Random Forest using the training data
    3: A list with the accuracy of Random Forest using the testing data
    """
    train_data_dir = os.path.join(data_dir, "train")
    test_data_dir = os.path.join(data_dir, "test")
    vocab_file_name = os.path.join(data_dir, "imdb.vocab")

    id3_train_results: list[float] = []
    id3_test_results: list[float] = []
    forest_train_results: list[float] = []
    forest_test_results: list[float] = []

    ignored_attrs = 12
    count_attrs = 200

    # get overfitting curve data
    for example_count in range(500, 10001, 500):
        result = main_test(train_data_dir, test_data_dir, vocab_file_name, example_count, count_attrs,
                           ignored_attrs)
        id3_train_results.append(result.id3_train_results.accuracy())
        id3_test_results.append(result.id3_test_results.accuracy())
        forest_train_results.append(result.forest_train_results.accuracy())
        forest_test_results.append(result.forest_test_results.accuracy())

    return id3_train_results, id3_test_results, forest_train_results, forest_test_results


def get_roc_data(data_dir: str) -> tuple[list[tuple[float, float]], list[tuple[float, float]],
                                         list[tuple[float, float]], list[tuple[float, float]]]:
    """
    Get the precision and recall of repeated executions of the ID3 and Random Forest classifiers with
    variable parameters.
    :param data_dir: the directory containing the training examples
    :return: A tuple containing 4 lists tuples holding 2 floats each. More specifically:
    0: A list with the precision and recall of the ID3 tree using the training data
    1: A list with the precision and recall of the ID3 tree using the testing data
    2: A list with the precision and recall of Random Forest using the training data
    3: A list with the precision and recall of Random Forest using the testing data
    """
    train_data_dir = os.path.join(data_dir, "train")
    test_data_dir = os.path.join(data_dir, "test")
    vocab_file_name = os.path.join(data_dir, "imdb.vocab")

    id3_train_results: list[tuple[float, float]] = []
    id3_test_results: list[tuple[float, float]] = []
    forest_train_results: list[tuple[float, float]] = []
    forest_test_results: list[tuple[float, float]] = []

    # get ROC curve data
    for ignored_attrs in range(0, 201, 50):
        for count_attrs in range(10, 500, 500):
            for example_count in range(500, 10001, 5000):
                results = main_test(train_data_dir, test_data_dir, vocab_file_name, example_count, count_attrs,
                                    ignored_attrs)
                id3_train_results.append((results.id3_train_results.precision(), results.id3_train_results.recall()))
                id3_test_results.append((results.id3_test_results.precision(), results.id3_test_results.recall()))
                forest_train_results.append((results.forest_train_results.precision(),
                                             results.forest_train_results.recall()))
                forest_test_results.append((results.forest_test_results.precision(),
                                            results.forest_test_results.recall()))

    return id3_train_results, id3_test_results, forest_train_results, forest_test_results


def overfit_to_file(data_dir: str, output_file_name: str) -> None:
    """
    Write the formatted overfit data into an output file.
    :param data_dir: the directory containing the training examples
    :param output_file_name: The name the output will be saved to
    """
    data = get_overfit_data(data_dir)
    separators = ["Training id3 accuracy:\n", "Testing id3 accuracy:\n", "Testing random forest accuracy:\n",
                  "Testing random forest accuracy:\n"]
    out_str = ""
    for i, separator in enumerate(separators):
        out_str += separator
        for accuracy in data[i]:
            out_str += str(accuracy) + ", "
        out_str = out_str[:-1]  # remove last comma
        out_str += "\n"

        with open(output_file_name, mode="w") as out_of:
            out_of.write(out_str)


def roc_to_file(data_dir: str, output_file_name: str):
    """
    Write the formatted precision / recall data into an output file.
    :param data_dir: the directory containing the training examples
    :param output_file_name: The name the output will be saved to
    """
    data = get_roc_data(data_dir)
    separators = ["Training id3 precision/recall:\n", "Testing id3 precision/recall:\n",
                  "Testing random forest precision/recall:\n", "Testing random forest precision/recall:\n"]
    out_str = ""
    for i, separator in enumerate(separators):
        out_str += separator
        for precision, recall in data[i]:
            out_str += f"({str(precision)}, {str(recall)}), "
        out_str = out_str[:-1]  # remove last comma
        out_str += "\n"

        with open(output_file_name, mode="w") as out_of:
            out_of.write(out_str)


if __name__ == "__main__":
    try:
        overfit_to_file(sys.argv[1], sys.argv[2])
        roc_to_file(sys.argv[1], sys.argv[2])
    except IOError as ioe:
        print("Error: ", ioe)
