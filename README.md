Ronald Keating\n
MATH 415

Note: Only use uppercase values for this. Could implement lowercase, but just wanted
to show how Enigma works through C implementation.

plugboard.txt
Format (from start of file):

    A B C D E F G H I J
    K L M N O P Q R S T

enigma.c
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

Looks something like:

    plugboard function on input_char
    where rotor1 = input_char
    value at rotor2 where input_char - 65
    input_char = character at rotor2[input_char - 65]
    value at rotor3 where input_char - 65
    input_char = character at rotor3[input_char - 65]
    value at reflector where input_char - 65
    input_char = character at reflector[input_char - 65]
    input_char = swap_reflector_characters

And then work backwards to find rotor1 value, which is then passed through
the plugboard function
