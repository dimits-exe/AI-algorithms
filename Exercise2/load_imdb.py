import os, re

class WordInfo:
    """ A class holding information about the frequency of a word """

    def __init__(self, word):
        self._word = word   # not used but could be there for completeness
        self.positive_occurences = 0
        self.frequency = 1

    def get_positive_chance(self):
        """
        Returns the chance the word is used in a positive context. For example
        if a word is used 10 times, three of which occurred a positive review,
        the method would returns 0.3

        Returns:
            A float between 0 and 1
        """
        return self.positive_occurences / self.frequency

    def __str__(self):
        return "%s - %d - %d - %.5f" % (self._word, self.positive_occurences,
                self.frequency, self.get_positive_chance())

def get_training_data(dir_path, ignored_count, max_count, sample_size = 5000):
    """
    Reads text from the files in a directory and returns a dictionary with the
    M most frequent words and their wordinfo, ignoring the N most common ones.

    Parameters:
        dir_path (str): The path to the directory with all the training data
        ignored_count (int): How many of the most common words will be excluded
        max_count (int): How many words will be included
        sample_size (int): The maximum numbers of files read

    Returns:
        (dict) A dictionary with [word (str)] : [word_info (WordInfo)] records

    Throws:
        ValueError: if the ignored and max counts are negative
    """

    # validate parameters
    if ignored_count < 0 or max_count <= 0:
        raise ValueError("The max- and ignored-word-counts must be positive")

    # gather data
    training_data = dict()
    neg_dir_path = os.path.join(dir_path, "neg")
    pos_dir_path = os.path.join(dir_path, "pos")
    ignored_chars = ['"', "'", '.', ',', '>', '<', '\\', '/', '-', '(', ')',
            ';', ':', '?']

    print("Loading postive training data...")
    _read_reviews(training_data, neg_dir_path, False, sample_size // 2,
        ignored_chars)

    print("Loading negative training data...")
    _read_reviews(training_data, pos_dir_path, True, sample_size // 2,
        ignored_chars)

    print("Loading succesfull.")
    
    if len(training_data) <= ignored_count:
        return training_data

    # is this how Im supposed to write code in this cursed language? no, *this* is :sunglasses:
    # TODO: remove this comment after getting flexed on by superior code. more :sunglasses:

    # sort all the frequencies to find the upper and lower threshold for words
    frequencies = [info.frequency for info in training_data.values()]
    sorted_frequencies = sorted(frequencies, reverse=True) # biggest to smallest
    frequency_count = len(sorted_frequencies)
    max_count = min(max_count, frequency_count - ignored_count)
    
    # start from `ignored_count` up to `ignored_count + max_count`
    # both thresholds are included in the range of acceptable frequencies
    upper_threshold = sorted_frequencies[ignored_count]
    lower_threshold = sorted_frequencies[ignored_count + max_count - 1]

    # return filtered results, k = word, v = wordinfo
    check_threshold = lambda freq: lower_threshold <= freq <= upper_threshold
    return {k:v for k, v in training_data.items() if check_threshold(v.frequency)}

def get_testing_data(directory_path):
    """
    Return a dictionary containing the contents of every data file.

    Parameters:
        directory_path (string): The path to the directory containing ALL the testing data files

    Returns:
        (dict) A dictionary with [review (string)] : [is_positive (bool)] records
    """

def _read_reviews(wordinfo_dict, dir_path, is_positive, sample_size, ignored_chars):
    """
    Opens all files in a directory and appends [word] : [WordInfo] records in
    the provided dictionary.

    Parameters:
        wordinfo_dict (dict): The dictionary to which the results of the file
            scan will be added.
        dir_path (str): The directory containing the training data
        is_positive (bool): Indicates if the directory contains positive reviews
        ignored_chars (list): The characters that will be removed from the text
    """

    files = os.listdir(dir_path)
    ignored_chars_pattern = re.compile("[%s\\d]" % (re.escape("".join(ignored_chars))))
    one_tenth_progress = min(len(files), sample_size) // 10
    
    for count, file in enumerate(files):
        # check if file limit has been reached
        if count == sample_size:
            break

        # read file contents
        try:
            file_path = os.path.join(dir_path, file)
            f_handle = open(file_path, mode='r', encoding= "utf8")
            contents = f_handle.read().lower()
        finally:
            f_handle.close()

        # clean contents
        clean_contents = ignored_chars_pattern.sub("", contents, 0)
        clean_contents = re.sub("\\s+", " ", clean_contents)

        # for each text that changes look if everything is ok regarding ignored
        # TODO: remove
        """
        print(contents)
        print()
        print(clean_contents)
        input()
        print()
        """

        # update frequencies
        for word in clean_contents.split(" "):
            wordinfo = wordinfo_dict.setdefault(word, WordInfo(word))
            wordinfo.frequency += 1
            if is_positive:
                wordinfo.positive_occurences += 1

        # show % progress
        if (count + 1) % one_tenth_progress == 0:   # `count` is 0-indexed
            print("%d%% complete..." % ((count + 1) // one_tenth_progress * 10))

def main():
    import time

    path_to_file = os.path.dirname(os.path.abspath(__file__))
    directory = os.path.join(path_to_file, "aclImdb", "train")

    try:
        start = time.perf_counter()
        results = get_training_data(directory, 5, 10, 10000)
        end = time.perf_counter()

    except os.error as err:
        print(f"Process didn't complete normally due to: {err}")

    else:
        get_freq = lambda item: item[1].frequency
        items_by_freq = sorted(results.items(), key = get_freq, reverse=True)

        for word, wordinfo in items_by_freq:
            print(word, wordinfo.frequency)

        print(f"Process ended after {end-start} seconds.")

if __name__ == "__main__":
    main()
