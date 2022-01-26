from unicodedata import category
from load_imdb import load_examples, get_attributes
from id3 import Category

class TestStats:
    def __init__(self, true_positives: int, false_positives: int, false_negatives: int, b: float = 1):
        self.true_positives = true_positives
        self.false_positives = false_positives
        self.false_negatives = false_negatives
        self.b = b

    def accuracy(self, true_positives: int, data_size:int) -> float:
        """
        A metric representing the % of correctly identified test data.

        Parameters:
            true_positives (int): the number of correct guesses
            data_size (int): the number of total guesses
        
        Returns:
            a value between [0,1] representing the accuracy of the test
        """
        return true_positives / data_size

    def precision(self, true_positives: int, false_positives: int) -> float:
        """
        A metric representing how many of the classified examples truly
        belong to their class in a 2-class model.

        Parameters:
            true_positives (int): the number of correct guesses
            false_positives (int): the number of examples classified as positive 
            even though they were negative
        
        Returns:
            a value between [0,1] representing the precision of the test
        """
        return true_positives / (true_positives + false_positives)

    def recall(self, true_positives:int, false_negatives: int) -> float:
        """
        A metric representing how many of the true members of a class
        were classified as that class in a 2-class model.

        Parameters:
            true_positives (int): the number of correct classifications
            false_negatives (int): the number of examples classified as negative
            even though they were positive
        
        Returns:
            a value between [0,1] representing the recall of the test
        """
        return true_positives / (true_positives + false_negatives)

    def f_measure(self, true_positives: int, false_positives: int, false_negatives: int, b: float = 1) -> float:
        """
        A metric describing the overall accurracy of a test by combining the
        recall and precision values.

        Parameters:
            true_positives (int): the number of correct guesses in the the test
            false_positives (int): the number of examples classified as positive 
            even though they were negative
            false_negatives (int): the number of examples classified as negative
            even though they were positive
            b (float): a weight between [0,1]. Values close to 0 emphasize precision, values close to 1 emphasize recall.

        Returns:
            a value representing the overall fitness of a class in a given test
        """
        precision_score = precision(true_positives, false_positives)
        recall_score = recall(true_positives, false_negatives)

        return ((b**2 + 1) * precision_score * recall_score) / (b**2 * precision_score * recall_score)



def test_classifier(classifier: object, directory: str, sample_size:int) -> TestStats:
    examples = load_examples(directory, sample_size)
    classifier.classify(examples)

    false_positives = 0
    false_negatives = 0
    true_positives = 0
    for example in examples:
        if example.actual == Category.POS and example.predicted ==  Category.POS:
            true_positives += 1
        elif example.actual == Category.POS and example.predicted == Category.NEG:
            false_negatives += 1
        elif example.actual == Category.NEG and example.predicted == Category.POS:
            false_positives += 1
    
    

    