from math import log2
from load_imdb import Review

def entropy(prob : float):
    """
    Returns the entropy of a category with 2 possible categories

    Parameters:
        prob (float): the probability of any word belonging to the category

    Returns:
        the entropy of the category
    """
    if prob == 0.0 or prob == 1.0:
        return 0
    else:
        return -prob * log2(prob) - (1-prob) * log2(1-prob)

def info_gain(positive_review_ratio: float, reviews: set[Review], word: str):
    """
    Returns the infomation gain of a word for a given category.

    Parameters:
        positive_review_ration (float): the ratio of positive to all reviews
        reviews (list[Review]): a collection containing reviews
        word (string): the word for which the information gain will be calculated

    Returns:
        The information gain of the word.
    """
    positive_reviews_with_word = 0
    negative_reviews_with_word = 0

    word_in_review_count = 0

    for review in reviews:
        if word in review.words:
            word_in_review_count += 1
            if review.is_positive:
                positive_reviews_with_word += 1
            else:
                negative_reviews_with_word += 1

    positive_chance_with_word = positive_reviews_with_word / (positive_reviews_with_word + negative_reviews_with_word)
    chance_of_word_in_any_review = word_in_review_count / len(reviews)

    #H(C) - P(X=1) * H(C|X=1) + P(X=0) * H(C|X=0)
    return entropy(positive_review_ratio) - chance_of_word_in_any_review * entropy(positive_chance_with_word) \
        + (1 - chance_of_word_in_any_review) * entropy(1 - positive_chance_with_word)


def choose_best_attr(words: set[str], reviews : set[Review]):
    """
    Returns the word with the maximum information gain out of a list of words.

    Parameters:
        words (string): the words to be examined for information gain in a specific review
        reviews (list[Review]): a list of all reviews currently loaded
    
    Returns:
        the word with the maximum information gain according to the provided reviews
    """
    #find the ratio of all positive reviews
    positive_reviews = 0
    negative_reviews = 0

    for review in reviews:
        if review.is_positive:
            positive_reviews += 1
        else:
            negative_reviews += 1

    ratio = positive_reviews / (positive_reviews + negative_reviews)

    #find the word with the maximum information gain
    max_gain = -1
    for word in words:
        gain = info_gain(ratio, reviews, word)
        if gain > max_gain:
            gain = max_gain
            max_gain_word = word
    
    return max_gain_word


def test():
    r1 = Review("owo uwu owo angry owo", False)
    r2 = Review("very normal words yes", True)
    r3 = Review("again angry owo and not so angry uwu", False)
    r4 = Review("very cool review yes yes", True)

    reviews = set()
    reviews.add(r1)
    reviews.add(r2)
    reviews.add(r3)
    reviews.add(r4)
    words = set(["owo", "yes", "again"])

    print(choose_best_attr(words, reviews))

if __name__ == "__main__":
    test()