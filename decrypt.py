#!/usr/bin/env python

# Script to decode polyalphabetic substitution cipher

import os
from atlas import maps

def decrypt_char(char, counter, maps):
    '''
    decrypt single character
    '''
    try:
        key_position = counter % 32 - 1
        decrypted_char = maps[char][key_position]
        return decrypted_char
    except:
        return '?'


def decrypt_page(page):
    '''
    decrypt list of strings
    '''
    decrypted_page = []
    for row, line in enumerate(page):
        decrypted_line = ''
        for column, char in enumerate(line):
            decrypted_char = decrypt_char(char,row + 1 + column, maps)
            decrypted_line += decrypted_char
        decrypted_page.append(decrypted_line)
    return decrypted_page


def get_filenames(path):
    return [os.path.splitext(file)[0] for file in os.listdir(path)]


def main():
    path = 'jackpot_q_bank'
    categories = get_filenames(path)
    for category in categories:
        page = open('jackpot_q_bank/%s.QQQ' % category, 'r').readlines()[1:] # ignore first metadata line
        page = [line[:-2] for line in page] # remove newline characters
        decrypted_page = decrypt_page(page)
        for line in decrypted_page: # print decrypted data
            print line


if __name__ == '__main__':
    main()
