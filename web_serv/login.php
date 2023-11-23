<?php
// Démarrage de la session
session_start();

// Connexion à la base de données SQLite
try {
    $db = new PDO('sqlite:/var/www/html/rfid-app/badges.db');
    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    die('Connection failed: ' . $e->getMessage());
}

// Vérification du type de requête (POST)
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Récupération des données du formulaire
    $username = $_POST['username'];
    $password = $_POST['password'];

    // Récupération du statut d'administrateur et de l'ID de l'utilisateur depuis la base de données
    $query = $db->prepare('SELECT id, is_admin FROM logins WHERE username = :username AND password = :password');
    $query->bindParam(':username', $username, PDO::PARAM_STR);
    $query->bindParam(':password', $password, PDO::PARAM_STR);
    $query->execute();
    $user = $query->fetch(PDO::FETCH_ASSOC);

    if ($user) {
        // Stockage de l'ID de l'utilisateur et du statut d'administrateur dans la session
        $_SESSION['user_id'] = $user['id'];
        $_SESSION['is_admin'] = $user['is_admin'];
        $_SESSION['username'] = $username; // Ajout du nom d'utilisateur dans la session

        // Redirection vers la page d'administration
        header("Location: admin_panel.php");
        exit();
    } else {
        // Identifiants incorrects, affichage d'un message d'erreur par exemple
        $error_message = "Identifiants incorrects";
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login</title>
    <link rel="stylesheet" href="styles.css"> <!-- Ajout du lien vers le fichier CSS -->
</head>
<body>
    <form method="post" action="login.php">
        <h2>Login</h2>

        <?php if (isset($error_message)) : ?>
            <p><?php echo $error_message; ?></p>
        <?php endif; ?>

        <label for="username">Username:</label>
        <input type="text" id="username" name="username" required>

        <label for="password">Password:</label>
        <input type="password" id="password" name="password" required>

        <button type="submit" class="center">Login</button>
    </form>
</body>
</html>
