# ParticleTRNG Author @Sean Donachiue
A simple true (true in that it continuously samples entropy rather than working from an initial seed) random number generator based conceptually on a particle simulation in 2-space, coded for windows using visual studio 2017 and the windows waveform library for audio recording. The intention was to create a cryptographically secure PRNG without the use of cryptographic primitives like existing hash functions which are commonly used for such applications. Cryptographic security is quite exhaustive to prove, and is beyond the scope of my mind. While this project has served mostly as an academic exercise, results obtained still measure up reasonably well to other random number generators.

## How it Works

The system is a commandline application for windows which takes input in the form [bytes/output value] [number of outputs to generate]. It is not recommended to generate outputs in batches of more than 250 at a time, as the audio buffer will become oversampled and cycles will be produced in the outputs.

One second of audio is recorded to a char* buffer. This is then sampled iteratively using variance in the system clock between iterations to as well as the iteration number to sample different values from the audio buffer. These sampled values are then used to initialize particles with positions wrapped to the range 0-63 and velocities to 1-256. As one might expect, Velocity * change-in-time is added to position for each particle on each iteration. Outputs are "pulled" from the system one at a time on each iteration time by indexing into the particle container (an STL vector) using the low order clock digits (in microseconds) in the change-in-time value from the last iteration, modulo the size of the particle container. As a result, the indices for output selection are somewhat variable.

Outputs are restricted to a base 64 charset by indexing position values into the following string 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

## How it Performs

[10,000 32-byte outputs](/testOutputs.txt) from this system were benchmarked for randomness using the [ENT test suite](http://www.fourmilab.ch/random/). Results obtained (and their interpretation) are as follows:

#### Entropy = 5.953667 bits per byte. 
This result isn't surprising, since base64 requires only 6 bits to encode. Therefore, its quite a good outcome.

#### Optimum compression would reduce the size of this 320000 byte file by 25 percent. 
Similarly, 6 bits/8 bits is 75%, so this is the best possible value for a base64 encoding of random values.

#### Chi square distribution for 320000 samples is 1055143.87, and randomly would exceed this value less than 0.01 percent of the times.
Essentially a failure on the chi-square distribution, the most stringent test of randomness in the suite. For reference, a p-value indicating a system is likely random would be anything greater than 0.05 or less than 0.95. A "good" value for a pseudo-random number generator is 0.97, (from the ENT landing page describing the tests)

#### Arithmetic mean value of data bytes is 85.7160 (127.5 = random). 
The mean value for a base64 encoding is 85. 127.5 is the mean value for a standard 8-bit char. Therefore, this is a great result.

#### Monte Carlo value for Pi is 4.000000000 (error 27.32 percent). 
This test plots bytes of output in a unit square, which circumscribes a unit circle, then uses these points to approximate pi by the equation pi = 4 * number of pts inside circle/total number of pts. The 4.000000000 result comes from the fact that all points are interpreted to be inside the circle, since the range of base 64 values never allows points outside the circle. Therefore, this formulation of the test can be dismissed.

#### Serial correlation coefficient is 0.006127 (totally uncorrelated = 0.0). 
This test measures the correlation between adjacent bits. This is a fairly good value, however, it is still not within the range of -0.002 - 0.002 (calculated using equations from Knuth, which you can find in my [paper](../Pseudorandom Numbers Generated via Simple Particle Simulation Evaluated for use in Cryptographic Applications.docx) on the initial (quite flawed) implementation of this system, which would indicate with 95% confidence that there is no correlation between adjacent values.

Contained also in the paper is a cryptanalyis of this system for susceptibility to input and state compromise extension attacks, as well as general discussion of PRNGs in use and their properties.
