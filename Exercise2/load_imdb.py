import os, re

class WordInfo:
    """
    A class holding information about the use of an associated word.
    """

    def __init__(self):
        self.positive_occurences = 0
        self.frequency = 1

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

    def __str__(self):
        return str(self.get_positive_chance())[:5] + "%"

def get_training_data(directory_path, ignored_words_count, max_words_count, sample_size = 5000):
    """
    Read the data file and return a dictionary with all the words contained within,
    ignoring the N most common words.

    Parameters:
        directory_path (string): The path to the directory containing ALL the training data files
        ignored_words_count (int): The N most common words not to be included in the dictionary
        max_words_count (int): The max number of most common words stored in the dictionary
        sample_size (int): The max numbers of files read

    Throws:
        ValueError if the int arguments are negative or max_words_count is smaller than
        ignored_words_count (an empty dictionary would always be returned)

    Returns:
        (dict) A dictionary with [word (string)] : [chance_of_being_positive (float)] records
    """
    ignored_tokens_regex = re.compile("\"|,>|<|\\|/|-")

    #check parameters
    if ignored_words_count < 0 or max_words_count <= 0:
        raise ValueError("The calling parameters need to be positive")
    if max_words_count < ignored_words_count:
        raise ValueError("The returned word count must be higher than the ignored word count")

    #gather data
    training_data = dict()
    print("Loading postive training data...")
    _read_reviews(training_data, os.path.join(directory_path, "neg"), False, 
        sample_size // 2, ignored_tokens_regex)

    print("Loading negative training data...")
    _read_reviews(training_data, os.path.join(directory_path, "pos"), True, 
        sample_size // 2, ignored_tokens_regex)

    print("Loading succesfull.")

    #get the N most common words
    most_common_words = set(sorted(list(training_data.values()), 
        key= lambda x: x.frequency)[ignored_words_count:])
    #is this how Im supposed to write code in this cursed language?

    #filter results
    filtered_data = dict()

    for data_item in training_data.items():
        if len(training_data) <= 0 or len(filtered_data) > max_words_count:
            break

        if data_item not in most_common_words:
            filtered_data[data_item[0]] = data_item[1]

    return filtered_data


def get_testing_data(directory_path):
    """
    Return a dictionary containing the contents of every data file.

    Parameters:
        directory_path (string): The path to the directory containing ALL the testing data files

    Returns:
        (dict) A dictionary with [review (string)] : [is_positive (bool)] records
    """
    

def _read_reviews(dictionary, directory_path, is_positive, sample_size, ignored_tokens_regex):
    """
    Open all files in the given directory and append [word] : [WordInfo]
    records in the provided dictionary.

    Parameters:
        dictionary (dict): The dictionary in which the results of the file scan will be appended to.
        directory_path (string): The path to the directory containing the
        positive / negative testing data files
        is_positive (bool): Whether or not the given directory contains positive reviews
        ignored_tokens_regex (Pattern): a pattern which is used to clean strings of impure content
        such as dots, commas, etc

    Throws:
        OSError if the directory isn't valid
    """
    def clean_string(string): #TODO: implement correct regex
        #return ignored_tokens_regex.sub(string, "")
        return string

    files_read = 0
    percent_progress = 0

    files = os.listdir(directory_path)
    for file in files:
        # read file contents
        try:
            f_handle = open(os.path.join(directory_path, file), mode='r', encoding= "utf8")
            contents = f_handle.read().lower()
        except PermissionError:
            print("Ignored file " + file + " because of missing permissions")
        except os.error as err: #ignore all files with errors
            print("Ignored file " + file + " because of " + str(err))
        finally:
            f_handle.close()

        for word in contents.split():
            word = clean_string(word)

            if word not in dictionary:
                dictionary[word] = WordInfo()   #create record
            else:
                dictionary[word].frequency += 1 #update record
            if is_positive:
                dictionary[word].positive_occurences += 1
        files_read += 1

        #show % progress
        if files_read % (min(len(files), sample_size) // 10) == 0:
            print(percent_progress*10, "% complete...")
            percent_progress += 1

        #check if file limit is reached
        if files_read > sample_size:
            break

if __name__ == "__main__":
    import time

    p = os.path
    #search in the current directory
    directory = p.join(p.join(p.dirname(p.abspath(__file__)), "aclImdb"), "train")

    start = time.time()
    results = get_training_data(directory, 20, 50, 1000)
    end = time.time()

    for item in results.items():
        print(item[0], item[1])

    print("Process ended after ", end - start, " seconds.")
