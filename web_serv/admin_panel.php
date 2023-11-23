<?php
session_start();

// Vérifie si l'utilisateur est connecté, sinon redirige-le vers la page de connexion
if (!isset($_SESSION['username'])) {
    header("Location: login.php");
    exit();
}

// Vérifie si l'utilisateur est administrateur, sinon redirige-le vers une page non autorisée
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

// Fonction pour récupérer la liste des cartes RFID
function getCardList($db) {
    $query = $db->query('SELECT * FROM badges');
    return $query->fetchAll(PDO::FETCH_ASSOC);
}

// Fonction pour ajouter un log
function addLog($db, $userId, $action, $details = null) {
    $query = $db->prepare('INSERT INTO logs (user_id, action, details) VALUES (:user_id, :action, :details)');
    $query->bindParam(':user_id', $userId, PDO::PARAM_INT);
    $query->bindParam(':action', $action, PDO::PARAM_STR);
    $query->bindParam(':details', $details, PDO::PARAM_STR);
    $query->execute();
}

// Traitement du formulaire (ajout, suppression et modification)
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['form_action'])) {
        $form_action = $_POST['form_action'];

        if ($form_action == 'add_card') {
            // Traitemer l'ajout d'une nouvelle carte
            $new_card = $_POST['new_card'];

            // Vérifie si la carte existe déjà
            $existing_cards = array_column(getCardList($db), 'id');
            if (!in_array($new_card, $existing_cards)) {
                // Ajouter la nouvelle carte à la base de données avec le statut "Refusé" par défaut
                $query = $db->prepare('INSERT INTO badges (id, status) VALUES (:id, :status)');
                $query->bindValue(':id', $new_card, PDO::PARAM_STR);
                $query->bindValue(':status', 'Refusé', PDO::PARAM_STR);
                $query->execute();

                // Ajouter un log pour l'ajout de carte
                addLog($db, $_SESSION['user_id'], 'Ajout de carte RFID', "Carte ajoutée : $new_card");
            }

        } elseif ($form_action == 'remove_card') {
            // Traiter la suppression d'une carte
            if (isset($_POST['submit_remove_card'])) { // Vérifier si le bouton Supprimer la carte a été soumis
                $selected_card_delete = $_POST['selected_card_delete'];

                // Supprimer la carte de la base de données
                $query = $db->prepare('DELETE FROM badges WHERE id = :id');
                $query->bindParam(':id', $selected_card_delete, PDO::PARAM_STR);
                $query->execute();

                // Ajouter un log pour la suppression de carte
                addLog($db, $_SESSION['user_id'], 'Suppression de carte RFID', "Carte supprimée : $selected_card_delete");
            }

        } elseif ($form_action == 'update_status') {
            // Traiter la modification du statut d'une carte
            $selected_card = $_POST['selected_card'];
            $new_status = $_POST['new_status'];

            // Mettre à jour le statut de la carte dans la base de données
            $query = $db->prepare('UPDATE badges SET status = :status WHERE id = :id');
            $query->bindParam(':status', $new_status, PDO::PARAM_STR);
            $query->bindParam(':id', $selected_card, PDO::PARAM_STR);
            $query->execute();

            // Ajouter un log pour la modification du statut de carte
            addLog($db, $_SESSION['user_id'], 'Modification du statut de carte RFID', "Nouveau statut : $new_status pour la carte : $selected_card");
        }
    }
}
?>

<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Panel Admin</title>
    <link rel="stylesheet" href="admin_styles.css">
</head>
<body>
    <div class="navbar">
        <div class="container">
            <a href="view_logs.php">Voir les logs</a>
            <a href="logout.php">Déconnexion</a>
        </div>
    </div>

    <div class="container">
        <h2>Panel Admin</h2>
        <span>Bienvenue, <?php echo $_SESSION['username']; ?>!</span>
        <h3>Liste des cartes RFID</h3>

        <form method="post" action="admin_panel.php">
            <label for="selected_card">Sélectionner une carte :</label>
            <select id="selected_card" name="selected_card">
                <?php
                $card_list = getCardList($db);
                foreach ($card_list as $card) {
                    echo "<option value='{$card['id']}'>{$card['id']}</option>";
                }
                ?>
            </select>

            <label for="new_status">Nouveau statut :</label>
            <select id="new_status" name="new_status">
                <option value="Autorisé">Autorisé</option>
                <option value="Refusé">Refusé</option>
            </select>

            <input type="hidden" name="form_action" value="update_status"> <!-- Champ caché pour distinguer l'action -->
            <button type="submit">Modifier le statut</button>
        </form>

        <form method="post" action="admin_panel.php">
            <label for="selected_card_delete">Sélectionner une carte à supprimer :</label>
            <select id="selected_card_delete" name="selected_card_delete">
                <?php
                foreach ($card_list as $card) {
                    echo "<option value='{$card['id']}'>{$card['id']}</option>";
                }
                ?>
            </select>

            <input type="hidden" name="form_action" value="remove_card"> <!-- Champ caché pour distinguer l'action -->
            <button type="submit" name="submit_remove_card">Supprimer la carte</button>
        </form>

        <h3>Ajouter une nouvelle carte RFID</h3>

        <form method="post" action="admin_panel.php">
            <label for="new_card">Nouvelle carte :</label>
            <input type="text" id="new_card" name="new_card" required>
            <input type="hidden" name="form_action" value="add_card"> <!-- Champ caché pour distinguer l'action -->
            <button type="submit">Ajouter la carte</button>
        </form>
    </div>
</body>
</html>
