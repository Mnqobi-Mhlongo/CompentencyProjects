from cs50 import get_string
while True:
    text = get_string("Text: ")
    length = len(text)

    # Get number of letters in text#
    letters = 0
    for i in range(length):
        if text[i] != ' ' and text[i] != "!" and text[i] != "." and text[i] != "?" and text[i] != "-" and text[i] != ";" and text[i] != "'" and text[i] != "," and text[i] != "(" and text[i] != ")":
            letters = letters + 1

    # Get number of word in text#
    words = 0
    for i in range(length):
        if text[i] == ' ':
            words = words + 1
    words = words + 1

    # Get number of sentences#
    sentences = 0
    for i in range(length):
        if text[i] == "!" or text[i] == "." or text[i] == "?":
            sentences = sentences + 1

    # Computee Coleman-Liau Index#
    L = float(letters / words) * 100
    S = float(sentences / words) * 100

    CLI = float(0.0588 * L - 0.296 * S - 15.8)

    # Print output#
    if CLI >= 16:
        print("Grade 16+")
    elif CLI < 1:
        print("Before Grade 1")
    else:
        print("Grade: ")
        print(round(CLI))

    break
