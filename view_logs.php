<?php
session_start();

// Vérifiez si l'utilisateur est connecté, sinon redirigez-le vers la page de connexion
if (!isset($_SESSION['username'])) {
    header("Location: login.php");
    exit();
}

// Vérifiez si l'utilisateur est administrateur, sinon redirigez-le vers une page non autorisée
if (!isset($_SESSION['is_admin']) || $_SESSION['is_admin'] !== 1) {
    header("Location: unauthorized.php");
    exit();
}

// Connexion à la base de données SQLite
try {
    $db = new PDO('sqlite:/var/www/html/rfid-app/badges.db');
    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    die('Connection failed: ' . $e->getMessage());
}

// Fonction pour récupérer la liste des logs avec les noms d'utilisateur
function getLogsList($db) {
    $query = $db->query('SELECT logs.id, logins.username, logs.action, logs.details, logs.timestamp FROM logs INNER JOIN logins ON logs.user_id = logins.id');
    return $query->fetchAll(PDO::FETCH_ASSOC);
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>View Logs</title>
</head>
<body>
    <h2>Logs</h2>
    <p>Bienvenue, <?php echo $_SESSION['username']; ?>!</p>
    <p><a href="logout.php">Logout</a></p>

    <h3>Liste des logs</h3>

    <table border="1">
        <tr>
            <th>ID</th>
            <th>Utilisateur</th>
            <th>Action</th>
            <th>Détails</th>
            <th>Date et heure</th>
        </tr>
        <?php
        $logs_list = getLogsList($db);
        foreach ($logs_list as $log) {
            echo "<tr>";
            echo "<td>{$log['id']}</td>";
            echo "<td>{$log['username']}</td>";
            echo "<td>{$log['action']}</td>";
            echo "<td>{$log['details']}</td>";
            echo "<td>{$log['timestamp']}</td>";
            echo "</tr>";
        }
        ?>
    </table>
</body>
</html>
