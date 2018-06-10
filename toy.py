from typing import Union


def parse_binary_sexp(inp: bytes):
    """
    >>> parse_sexp("(+ 5 (+ 3 5))")
    [['+', '5', ['+', '3', '5']]]

    """
    sexp = [[]]
    all_digits = True
    word = b''
    skip = 0
    stack_size = 0
    for i, char in enumerate(inp):
        char = bytes([char])
        print(i, char)
        if skip:
            print('skip')
            skip -= 1
            continue
        if char == b'(':
            if word:
                sexp[-1].append(word)
                word = b''
            sexp.append([])
            all_digits = True
            stack_size += 1
            print('open')
        elif char == b')':
            if word:
                sexp[-1].append(word)
                word = b''
            temp = sexp.pop()
            sexp[-1].append(temp)
            stack_size -= 1
            print('close')
        elif char in b' \n\t':
            if word:
                sexp[-1].append(word)
                word = b''
            print('space')
            all_digits = True
        else:
            if char == b':' and all_digits:
                skip = int(word.decode('utf-8'))
                sexp[-1].append(inp[i + 1:i + 1 + skip])
                print('NUMBER {}'.format(sexp[-1]))
                word = b''
                continue
            if char not in b'0123456789':
                all_digits = False
            word += char
            print('char = {} all_digits = {}'.format(char, all_digits))
    assert stack_size == 0
    assert not word
    return sexp[0][0]


def parse_sexp(string):
    if isinstance(string, str):
        return parse_binary_sexp(string.encode('utf-8'))
    return parse_binary_sexp(string)


if __name__ == '__main__':
    inp = "(genkey(ecc(curve 9:secp256k1)(flags nocomp)))"
    out = parse_sexp(inp)
    print("inp = {}".format(inp))
    print("out = {}".format(out))

    inp = open('trace.bin', 'rb').read()
    out = parse_sexp(inp)
    print("inp = {}".format(inp))
    print("out = {}".format(out))
