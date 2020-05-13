from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # split each string into lines and remove duplicates
    lines_a = set(a.split("\n"))
    lines_b = set(b.split("\n"))

    return lines_a & lines_b


def sentences(a, b):
    """Return sentences in both a and b"""

    # split each string into sentences and remove duplicates
    sentences_a = set(sent_tokenize(a))
    sentences_b = set(sent_tokenize(b))

    return sentences_a & sentences_b


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # split each string into substrings and remove duplicates
    substrings_a = set(substr_tokenize(a, n))
    substrings_b = set(substr_tokenize(b, n))

    return substrings_a & substrings_b


def substr_tokenize(string, n):

    substrings = []

    for i in range(len(string) - n + 1):
        substrings.append(string[i: i + n])

    return substrings
