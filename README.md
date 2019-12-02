# Ronald Keating for MATH 415

Note: Only use uppercase values for this. Could implement lowercase, but just wanted
to show how Enigma works through C implementation.

## plugboard.txt
Format (from start of file):

    A B C D E F G H I J
    K L M N O P Q R S T

## enigma.c
This is an Enigma program that simulates how the actual Enigma machine works.
Given an input, the characters are sent through a plugboard swap and then to
the rotors. From here the rotors take the value of the character, subtract 65
(decimal value of 'A') to get some array value. This is the array value of the 
next rotor. The next rotor will have some randomized alphabet, where the same 
process will happen again. Once it reaches the reflector, the final rotor will
treat the reflector as another rotor. The reflector then has pairs that it
essentially swaps values with. From here, the program works backwords, finding 
the value of C given the array value. This then searches for the letter value of 
the previous rotor.

## overview of how it works
So the user enters some character, c, into the program (actually enters a string, but other than the rotors interating, there is no change on how the letters are encrypted/decrypted). We take the value of c (which is uppercase and A=65 in decimal) so we subtract 65 from our letter c, and in this case, set it equal to some temporary integer temp. This gives us a temp value between 0-25, which is exactly our array sizes. So we begin with array 1, or rotor 1 (we are calling it rotor 1 because it is the first rotor in this process, not the randomly chosen rotor by the user). So now, we can set c = array1[temp] (or rotor1[temp]). These character arrays are random, so you can begin to see how this will work throughout the rotors. Again, we take temp=c-65, giving us a number between 0-25. We will set c=array2[temp], and then repeat the process for the third array. For the reflector, we will just treat it like a previous array, setting c=reflector[temp]. For the actual reflection part, we have two separate "half" arrays. These numbers are the pairing set for the reflector (this was done in order to create randomization, eliminating a mathmatical function which could easily be broken in that case). So the character that we get from the reflector, we scan for it in the "half" arrays. If it is in "half" array1, then c equals whatever value is in the SAME location as "half" array2, and vice-versa. From here, we scan for wherever c is in reflector, and so now we are in some location in the reflector. From here we must work backwards. Instead of finding c-65 and receving our next location, we must scan the next array based off our current location. So lets say that we are in reflector position 2. Then we ADD 65 to two and search in teh next array where the letter C is (since C=65+2 in decimal). Do this all the way back to array1/rotor1, and we have completed enigma. Oh, and the plugboard is a simple swap function that happens before and after the rotor process I just explained.
