import os

class WordInfo:
    """
    A class holding information about the use of an associated word.
    """

    def __init__(self):
        self.positive_occurences = 0
        self.frequency = 0

    def get_positive_chance(self):
        """
        Get the chance the word is used in a positive context.
        For example if a word is used 10 times, three of which in a positive 
        review, the method would return 0.3

        Returns:
            A float in the [0,1] range or None if the word was never used.
        """
        if self.frequency == 0:
            return None
        else:
            return self.positive_occurences / self.frequency

def get_training_data(directory_path, ignored_words_count, max_words_count):
    """
    Read the data file and return a dictionary with all the words contained within,
    ignoring the N most common words.

    Parameters:
        directory_path (string): The path to the directory containing ALL the training data files
        ignored_words_count (int): The N most common words not to be included in the dictionary
        max_words_count (int): The max number of most common words stored in the dictionary

    Throws:
        ValueError if the int arguments are negative or max_words_count is smaller than
        ignored_words_count (an empty dictionary would always be returned)

    Returns:
        (dict) A dictionary with [word (string)] : [chance_of_being_positive (float)] records
    """
    #check parameters
    if ignored_words_count < 0 or max_words_count <= 0:
        raise ValueError("The calling parameters need to be positive")
    if max_words_count < ignored_words_count:
        raise ValueError("The returned word count must be higher than the ignored word count")

    #gather data
    results = dict()
    _read_reviews(results, os.path.join(directory_path, "neg"), False)
    _read_reviews(results, os.path.join(directory_path, "pos"), True)

    #get the N most common words
    most_common_words = set(sorted(list(results.values()), 
        key= lambda x: x.frequency)[ignored_words_count:])
    #is this how Im supposed to write code in this cursed language?

    #filter results
    filtered_results = dict()

    for item in results:
        if len(results) <= 0 and len(filtered_results) > max_words_count:
            break

        if item not in most_common_words:
            filtered_results[item.key()] = item.value()

        results.pop(item.key()) #release memory

    return filtered_results


def get_testing_data(directory_path):
    """
    Return a dictionary containing the contents of every data file.

    Parameters:
        directory_path (string): The path to the directory containing ALL the testing data files

    Returns:
        (dict) A dictionary with [review (string)] : [is_positive (bool)] records
    """
    

def _read_reviews(dictionary, directory_path, is_positive):
    """
    Open all files in the given directory and append [word] : [WordInfo]
    records in the provided dictionary.

    Parameters:
        dictionary (dict): The dictionary in which the results of the file scan will be appended to.
        directory_path (string): The path to the directory containing the
        positive / negative testing data files
        is_positive (bool): Whether or not the given directory contains positive reviews

    Throws:
        OSError if the directory isn't valid
    """
    for file in os.listdir(directory_path):
        # read file contents
        try:
            f_handle = open(os.path.join(dir, file), mode='r', encoding= "utf8")
            contents = f_handle.read().lower()
        except PermissionError:
            print("Ignored file " + file + " because of missing permissions")
        except os.error as err: #ignore all files with errors
            print("Ignored file " + file + " because of " + str(err))
        finally:
            f_handle.close()

        for word in contents.split():
            if word in dictionary:
                dictionary[word].frequency += 1
                if is_positive:
                    dictionary[word].positive_occurences += 1
            else:
                dictionary[word] = WordInfo()
