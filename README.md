# slotbot

One of the games is centered around answering ridiculously difficult general knowledge questions. It has no element of luck, and can be won by skill alone. I wanted to make a system that could use computer vision to consistently and undetectably win the jackpot prize.

I found the game ROM on the deepweb, and decrypted the question/answer bank. Using this, I made a system that could covertly read the screen and tell me the answer through a secret earpiece.

Here's the pipeline:

- Capture image of slot machine screen with buttonhole camera to rPi microcomputer
- Process image to undo perspective shift and segment into question and answer boxes with OpenCV
- Pass processed question boxes to Google Tesseract for text recognition
- Running read text through a hand-designed brute-force search to get the most likely answer.
- Pass answer through text-to-speech engine and into hidden earpiece.
- (profit)

The inner workings of the game are probably a little laborious, but the code to hack it is pretty cool, and makes use of some neat linear algebra tricks.

### The game

The game asks you a series of general knowledge questions. It presents you with a choice of four answers, where one is correct. The more you get right, the more money you build up, until you win the jackpot.

### Decrypting the game files
The game data files look like [this](https://github.com/jjough/slotbot/blob/master/jackpot_q_bank/UK_geography_01.QQQ); encrypted, unreadable text.
Bizzarely, and fortunately, it turned out that this was a [polyaplhabetic substitution cipher](https://en.wikipedia.org/wiki/Enigma_machine).
This means that we can fairly easily write a script to get a list of questions and answers in human-readable, decrypted form (run `python decrypt.py`).

### Designing a brute force search

Now we have the data, the fun begins. We need to read the screen, match what we can see to a question in the data bank via a brute force search, and read out the corresponding correct answer.

I initially tried to do this with the question data alone, ignoring the answers. Unfortunately, this doesn't work. Optical character recognition is imperfect, especially when running in real-time off a bad camera. About 30% of the characters will typically be misread (e.g. 'What is the name of ?' could be read as as something like 'Wlal  5s the nome af?'). This means that the OCR-read question text is typically too garbled to identify which exact question it corresponds to- we can only narrow the search down to about 30 canditates.

So, we need to use the information provided by the answers to help identify which question we are looking at. This makes the brute force search a little more tricky, but still possible.

The two basic ingredients of this brute-force search will be (i) a way to compare two strings for similarity; and, using this, (ii) a metric to rank similarity between imperfectly observed question/answer pairs, and true samples from the database.

There's a standard information-theoretic way of comparing two strings, which will work fine for this purpose: the [Levenschtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance), defined as the minimum number of edits needed to change one string into another. Since a longer string tends to accumulate more reading errors, we'll normalize the Levenschtein distance over its length.

Now we need to use this to build a metric comparing question/answer sets.

Here's the search algorithm that ended up working:

 = argmax
 
You can think of it as finding the lowest Levenshtein inner product between the observed question/answer set and all the possible question/answer sets in the databank.

This algorithm is especially nice because it's expressed in matrix form, and so can be evaluated quickly using linear algebra shortcuts.

Here's the code to carry out the brute force search given the output of the OCR.

### Hardware

I bought a [raspberry pi 2](https://www.raspberrypi.org/products/raspberry-pi-2-model-b/) microcomputer to run the software.

I actually couldn't get the code to run fast enough on the raspberry pi to be useful (a single pass took about 30s). The bottleneck was the computer vision and OCR (the only bits I couldn't optimize), so I ended up having to pipe the image over wifi to be processed by a laptop in my backpack. [Here's the code on the microcomputer](). I used a TTS engine to read out the answer into an earpiece.
