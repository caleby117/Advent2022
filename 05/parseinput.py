def main():
    stacks = [
        'DLVTMHF',
        'HQGJCTNP',
        'RSDMPH',
        'LBVF',
        'NHGLO',
        'WBDGRMP',
        'GMNRCHLO',
        'CLW',
        'RDLQJZMT'
    ]
    print('stacks initialised')
    with open('input.txt', 'w') as inputfile:
        print("opening file to write")
        inputfile.write(f'{len(stacks)}\n')
        for s in stacks:
            inputfile.write(f'{s}\n')
        with open('raw.txt', 'r') as rawfile:
            for line in rawfile.readlines():
                line = line.split(' ')
                inputfile.write(f'{line[1]} {line[3]} {line[5]}')


if __name__ == '__main__':
    main()