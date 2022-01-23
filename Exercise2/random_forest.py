from random import sample
from id3 import Example, Category, id3
from load_imdb import get_attributes

class RandomForest:
    """
    A Random Forest classifier internally using ID3 trees to classify examples.
    """

    def __init__(self, training_examples: set[Example], trees:int):
        """
        Create a new Random Forest classifier.

        Parameters:
            training_examples (set[Example]): the training data
            samples (int): the number of trees used in the classifier
        """
        tree_ls = []
        training_set_count = len(training_examples) / trees

        for example_sample in trees:
            example_sample = set(sample(training_examples, training_set_count))
            tree_ls.append(id3(example_sample, get_attributes(example_sample)))

        self.trees = tree_ls
    
    def classify(self, test_example: Example) -> Category:
        """
        Classifies the provided example by plurality vote of the 
        trees created during initialization.

        Parameters:
            test_example (Example): The example to be classified

        Returns:
            The estimated category of the example.
        """
        pos_examples = 0
        neg_examples = 0

        for tree in self.trees:
            category = tree.get_category(test_example)
            if category == Category.POS:
                pos_examples += 1
            elif category == Category.NEG:
                neg_examples += 1
        
        if pos_examples > neg_examples:
            return Category.POS
        else:
            return Category.NEG
