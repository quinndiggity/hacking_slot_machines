# slotbot
This was a dumb but profitable project .

One of the games is centered around answering ridiculously difficult general knowledge questions. It has no element of luck, and can be won by skill alone. I wanted to make a system that could use computer vision to consistently and undetectably win the jackpot prize.

I managed to find the game ROM on the deepweb, and decrypted the question/answer bank. Using this, I made a system that could covertly read the screen and tell me the answer through a secret earpiece.

Here's the pipeline:

- Capture image of slot machine screen with buttonhole camera to rPi microcomputer
- Process image to undo perspective shift and segment into question and answer boxes with OpenCV
- Pass processed question boxes to Google Tesseract for text recognition
- Running read text through a hand-designed brute-force search to get the most likely answer.
- Pass answer through text-to-speech engine and into hidden earpiece.
- (profit)

The inner workings of the game are probably a little laborious, but the code to hack it is pretty cool, and makes use of some neat linear algebra tricks.

### The game

The game asks you a series of general knowledge questions. It presents you with a choice of four answers, where one is correct. The more you get right, the more money you build up, until you win the jackpot. If you make a mistake, you lose. If you hesitate for more than ten seconds, you lose. Here's a photo from the game:


### Decrypting the game files
The game data files look like [this](); encrypted, unreadable text.
Bizzarely, and fortunately, it turned out that this was a [polyaplhabetic substitution cipher](https://en.wikipedia.org/wiki/Enigma_machine).
This means that we can fairly easily write a script to get a list of questions and answers in human-readable, decrypted form (run `python decrypt.py`).
When you do it, you find that there are about 15,000 questions in the database, and most of them look like like [this]() - a question with a list of about 20 answers (more on this later).

### Designing a brute force search

Now we have the data, the fun begins. We need to read the screen, match what we can see to a question in the data bank via a brute force search, and read out the corresponding correct answer.

I initially tried to do this with the question data alone, ignoring the answers. This does not work for two reasons:

- Firstly, optical character recognition is imperfect, especially when running in real-time off a bad camera. About 30% of the characters will typically be misread (e.g. 'What is the name of ?' could be read as as something like 'Wlal  5s the nome af?'). This means that the OCR-read question text is typically too garbled to identify which exact question it corresponds to. OCR is slow, too- since we'll be running this off a very small computer, it turns out that we can only afford about two or three readings every ten seconds. This means that we can't average out errors over multiple readings. The answers contain crucial additional information pertaining to the identity of the question which cannot be discarded.

- Secondly, the data bank contains a bunch of questions like [this one]() which have about 20 different possible answers. The game randomly picks a choice of four answers from the 20 each time it shows the question (more on this later). This means that there is not a one-to-one correspondence between questions and answers, so the text of the question alone isn't sufficient to figure out the correct answer- we need to read the answers, too.

So, we need to use the information provided by the answers to help identify which question we are looking at. This makes the brute force search a little more tricky, but still possible.

The two basic ingredients of this brute-force search will be (i) a way to compare two strings for similarity; and, using this, (ii) a metric to rank similarity between imperfectly observed question/answer pairs, and true samples from the database.

There's a standard information-theoretic way of comparing two strings, which will work fine for this purpose: the [Levenschtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance), defined as the minimum number of edits needed to change one string into another. Since a longer string tends to accumulate more reading errors, we'll normalize the Levenschtein distance over its length.

Now we need to use this to build a metric comparing question/answer sets. First, it'll be useful to count up how many combinations we need to search through.
The game chooses 4 answers from a list of about 20 possible choices. If it chose randomly, the number of combinations would scale superexponentially with the number of choices; and a naive brute-force search would need to search through 15, 000 * 20C4 = 7E7 possible combinations. This combinatorial explosion would probably be insurmountable for a naive search.
Bizzarely, and fortunately, it chooses five consecutive answers from the list of 20, and then randomly cuts one out. It then displays the remaining four on the screen. This means that the number of combinations is only linear in the number of choices, and the solution is in reach of a naive brute force search.

Here's the search algorithm I came up with:

 = argmax
 
You can think of it as finding the lowest Levenshtein inner product between the observed question/answer set and all the possible question/answer sets in the databank.

This algorithm is especially nice because it's expressed in matrix form, and so can be evaluated quickly using linear algebra shortcuts. These are implemented automatically by any BLAS-enabled linear albegra library - I used Numpy.

Here's the code to carry out the brute force search, given the output of the OCR.

### Hardware

I bought a [raspberry pi 2](https://www.raspberrypi.org/products/raspberry-pi-2-model-b/) microcomputer to run the software, and drilled out the center of a shirt button to cover the camera.

I actually couldn't get the code to run fast enough on the raspberry pi to be useful (a single pass took about 30s). The bottleneck was the computer vision and OCR (the only bits I couldn't optimize), so I ended up having to pipe the image over wifi to be processed by a laptop in my backpack. [Here's the code on the microcomputer]().

I used a TTS engine to read out the answer, and I bought an invisible ['exam ear'](http://www.examear.com/) induction loop and earpiece from china. It turned out to be really quiet, so I had to sew the loop into a jacket with a big flared collar.

I haven't included the code for the OpenCV and Tesseract stuff because (a) you need to install OpenCV and Tesseract to run it (these take ages to download); and (b) it's boring standard use that you can find in a basic [practical computer vision textbook](https://www.amazon.com/Mastering-OpenCV-Practical-Computer-Projects/dp/1849517827).

So, that's essentially the whole system. 

### The system in action

The thing actually worked, amazingly. Here's me winning the jackpot (this is unfortunately the only video I have).

This baby paid my rent for months.
