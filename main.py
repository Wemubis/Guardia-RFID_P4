import sqlite3
import sys
def mutual(card):
    connection = sqlite3.connect("badges.db")
    cursor = connection.cursor()

    cursor.execute("SELECT hash_card FROM hash")

    result = cursor.fetchall()

    connection_ok = 0

    for line in result:
        if card == line:
            connection_ok = 1
            break
        else:
            connection_ok = 0

    print(connection_ok)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <card_number>")
    else:
        card_number = sys.argv[1]
        mutual(card_number)