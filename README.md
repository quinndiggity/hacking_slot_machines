# SlotBot

This repo contains the code for a system to hack a particular brand of slot machine using computer vision and brute-force search.

The slot machine game is centered around answering difficult general knowledge questions. I found the game ROM online and decrypted the question/answer bank. Using this, I made a system that could covertly read the screen and tell me the answer through a secret earpiece.

Here's the pipeline:

- Capture image of slot machine screen with buttonhole camera to rPi microcomputer
- Process image to undo perspective shift and segment into question and answer boxes with OpenCV
- Pass processed question boxes to Google Tesseract for text recognition
- Running read text through a hand-designed brute-force search to get the most likely answer.
- Pass answer through text-to-speech engine and into hidden earpiece.
- (profit)

The inner workings of the game are probably a little laborious, but the code to hack it is pretty cool and makes use of some neat linear algebra tricks.

### The game

The game asks you a series of general knowledge questions. It presents you with a choice of four answers, where one is correct. The more you get right, the more money you build up, until you win the jackpot.

### Decrypting the game files
The game data files look like [this](https://github.com/jjough/slotbot/blob/master/jackpot_q_bank/UK_geography_01.QQQ); encrypted, unreadable text.
Bizzarely, and fortunately, it turned out that this was a [polyaplhabetic substitution cipher](https://en.wikipedia.org/wiki/Enigma_machine).
This means that we can fairly easily write a script to get a list of questions and answers in human-readable, decrypted form (run `python decrypt.py`).

### Designing a brute force search

Now we have the data, the fun begins. We need to read the screen, match what we can see to a question in the data bank via a brute force search, and read out the corresponding correct answer.

I initially tried to do this with the question data alone, ignoring the answers. Unfortunately, this doesn't work. Optical character recognition is imperfect, especially when running in real-time off a bad camera. About 30% of the characters will typically be misread. This means that the OCR-read question text is typically too garbled to identify which exact question it corresponds to; we can only narrow the search down to about 30 possible candidates.

So, we need to use the information provided by the answers to help identify which question we are looking at. This makes the brute force search a little more tricky, but still possible.

The two basic ingredients of this brute-force search will be (i) a way to compare two strings for similarity; and, using this, (ii) a metric to rank similarity between imperfectly observed question/answer pairs and true samples from the database.

We use the [Levenschtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance) to define the similarity between two strings, defined as the minimum number of edits needed to change one string into another. Since a longer string tends to accumulate more reading errors, we'll normalize the Levenschtein distance over its length.

We form a confusion matrix of OCR answers against database answers. Taking the Frobenious norm between this and every 4-D [permutation matrix](https://en.wikipedia.org/wiki/Permutation_matrix) will give us the metric we need. We use permutation matrices because the answers can be randomly ordered. We can brute-force search over this to find the correct answer. The intuition behind this algorithm is that we're taking the dot product between the observed confusion matrix (which is noisy due to poor observability), and idealized confusion matrices (assuming perfect observability). We use permutation matrices because the four answers can appear in any permutation.

This algorithm is especially nice because it's expressed in matrix form and so can be evaluated quickly using linear algebra shortcuts.

The code to carry out the brute force search can be found in `/src/`.

### Hardware

I bought a [raspberry pi 2](https://www.raspberrypi.org/products/raspberry-pi-2-model-b/) microcomputer to run the software, and used a TTS engine to read out the answer into an earpiece..

I actually couldn't get the code to run fast enough on the raspberry pi to be useful (a single pass took about 30s). The bottleneck was the computer vision and OCR (the only bits I couldn't optimize), so I ended up having to pipe the image over wifi to be processed by a laptop in a backpack. The code running on the microcomputer can be found in `./pi_interface.py`.


![](quiz.gif)
