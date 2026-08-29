import argparse
import csv

import matplotlib.pyplot as plt


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("csv_file", help="CSV file to plot")
    parser.add_argument("output_file", help="Output PNG file, for examplelambertian.png")
    args = parser.parse_args()

    x_values = []
    y_values = []

    with open(args.csv_file, newline="") as file:
        reader = csv.DictReader(file)

        headers = reader.fieldnames
        if headers is None or len(headers) < 2:
            raise ValueError("CSV file must have at least two columns with headers")

        x_name = headers[0]
        y_name = headers[1]

        for row in reader:
            x_values.append(float(row[x_name]))
            y_values.append(float(row[y_name]))

    plt.plot(x_values, y_values, marker="o")
    plt.xlabel(x_name.replace("_", " "))
    plt.ylabel(y_name.replace("_", " "))
    plt.title(args.output_file.rsplit(".", 1)[0].replace("_", " "))
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(args.output_file, dpi=200)


if __name__ == "__main__":
    main()
