<?php
// Démarrage de la session
session_start();

// Vérifie si l'utilisateur est connecté, sinon le redirige vers la page de connexion
if (!isset($_SESSION['username'])) {
    header("Location: login.php");
    exit();
}

// Vérifie si l'utilisateur est administrateur, sinon le redirige vers une page non autorisée
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
function getLogsList($db, $limit) {
    $query = $db->prepare('SELECT logs.id, logins.username, logs.action, logs.details, logs.timestamp FROM logs INNER JOIN logins ON logs.user_id = logins.id ORDER BY logs.id DESC LIMIT :limit');
    $query->bindParam(':limit', $limit, PDO::PARAM_INT);
    $query->execute();
    return $query->fetchAll(PDO::FETCH_ASSOC);
}

// Définir le nombre de lignes par défaut
$defaultLimit = 10;

// Vérifie si le formulaire a été soumis
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['form_action']) && $_POST['form_action'] == 'set_limit') {
        $selected_limit = $_POST['selected_limit'];
        // Mettre à jour le nombre de lignes à afficher dans la session
        $_SESSION['log_limit'] = $selected_limit;
    }
}

// Obtenez le nombre de lignes à afficher, s'il n'est pas défini, utilisez le nombre par défaut
$logLimit = isset($_SESSION['log_limit']) ? $_SESSION['log_limit'] : $defaultLimit;
?>

<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Consulter les Logs</title>
    <link rel="stylesheet" href="view_logs_styles.css">
</head>
<body>

    <!-- Barre de navigation -->
    <nav>
        <ul>
            <li><a href="admin_panel.php">Panneau d'Administration</a></li>
            <li><a href="logout.php">Déconnexion</a></li>
        </ul>
    </nav>

    <div class="container">
        <h2>Logs</h2>
        <p>Bienvenue, <?php echo $_SESSION['username']; ?>!</p>

        <!-- Formulaire pour choisir le nombre de lignes à afficher -->
        <form method="post" action="view_logs.php">
            <label for="selected_limit">Nombre de lignes à afficher :</label>
            <select id="selected_limit" name="selected_limit">
                <option value="5" <?php echo ($logLimit == 5) ? 'selected' : ''; ?>>5</option>
                <option value="10" <?php echo ($logLimit == 10) ? 'selected' : ''; ?>>10</option>
                <option value="15" <?php echo ($logLimit == 15) ? 'selected' : ''; ?>>15</option>
            </select>
            <input type="hidden" name="form_action" value="set_limit">
            <button type="submit">Appliquer</button>
        </form>

        <h3>Liste des logs</h3>

        <table>
            <tr>
                <th>ID</th>
                <th>Utilisateur</th>
                <th>Action</th>
                <th>Détails</th>
                <th>Date et heure</th>
            </tr>
            <?php
            $logs_list = getLogsList($db, $logLimit);
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
    </div>
</body>
</html>
