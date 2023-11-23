import sqlite3

card = ""

connection = sqlite3.connect("base_de_donnee.db")

cursor = connection.cursor()

cursor.execute("SELECT * FROM hash")

result = cursor.fetchall()

connection_ok = 0

for line in result:
    if card == line:
        connection_ok = 1
        break
    else:
        connection_ok = 0

print(connection_ok)
