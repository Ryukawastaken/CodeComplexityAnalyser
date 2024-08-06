import re
import csv

def analyze_file(filename):
    operators = ['+', '++', '-', '--', '+=', '-=', '*', '*=', '/', '/=', '%', '%=', '==', '=', '!', '!=', '<', '<=',
                 '>', '>=', '<=>', '^', '&&', '||', '&', 'and', 'or', 'not', '(', '[', '{', ',', '::', 'sizeof', 'new',
                 'delete', '#', "using", 'const', 'enum', 'int', 'float', 'string', 'bool', 'struct', ';', 'typedef',
                 'case', ":", "break", "continue", "void", "return", "else"]

    uniqueOperators = ["Player", "Target", "Game_State", "NPC_List", "Game", "color", "triangle", "rectangle"]

    operators.extend(uniqueOperators)  # Extend the operators list with uniqueOperators

    # Define regex pattern to identify named variables, primitives, and operators
    pattern = r'(\w+|"[^"]*"|[\+\-\*\/%=<>!&\^\|]+|;)|(\s+)'

    FunctionPattern = r'(\w+)(?=\()|\w+|"[^"]*"|[\+\-\*\/%=<>!&\^\|]+|;|\(|\)'

    with open(filename, 'r') as file:
        code = file.read()

    tokens = re.findall(FunctionPattern, code)

    used_operators = {}
    operands_count = {}

    for token in tokens:
        if token != '':
            used_operators[token] = used_operators.get(token, 0) + 1

    with open(filename, 'r') as file:
        lines = file.readlines()

        for line in lines:
            # Remove comments
            line = re.sub(r'\/\/.*|\/\*(.|\n)*?\*\/', '', line)
            # Remove whitespace
            line = line.strip()
            # Split line into tokens
            tokens = re.findall(pattern, line)

            for token_group in tokens:
                token = token_group[0]
                if token:
                    if token in operators:
                        used_operators[token] = used_operators.get(token, 0) + 1
                    elif re.match(r'^[\w"]+$', token):
                        if (token not in used_operators):
                            operands_count[token] = operands_count.get(token, 0) + 1

    # Calculate total counts
    total_operators = len(used_operators)
    total_operands = sum(operands_count.values())

    # Create lists to store data for CSV
    data = []
    for operator, count in used_operators.items():
        data.append([operator, 'Operator', count])
    for operand, count in operands_count.items():
        data.append([operand, 'Operand', count])

    return data, used_operators, operands_count, total_operators, total_operands


def main():
    filename = input("Enter the name of the text file to analyze: ")
    data, operators_count, operands_count, total_operators, total_operands = analyze_file(filename)

    print("Distinct operators and their counts:")
    for operator, count in operators_count.items():
        print(f"{operator}: {count}")

    print("\nDistinct operands and their counts:")
    for operand, count in operands_count.items():
        print(f"{operand}: {count}")

    print("\nTotal number of operators:", total_operators)
    print("Total number of operands:", total_operands)

    # Write results to CSV
    with open('analysis_results.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['Token', 'Type', 'Count'])
        for row in data:
            writer.writerow(row)


    print("Analysis results written to analysis_results.csv")


if __name__ == "__main__":
    main()
