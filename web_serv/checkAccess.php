<?php
$cardId = isset($_GET['cardId']) ? $_GET['cardId'] : '';

// Connexion à la base de données SQLite
try {
    $db = new PDO('sqlite:/var/www/html/rfid-app/badges.db');
    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    die('Connection failed: ' . $e->getMessage());
}

// Vérification du statut dans la base de données
$query = $db->prepare('SELECT * FROM badges WHERE id = :cardId');
$query->bindParam(':cardId', $cardId, PDO::PARAM_STR);
$query->execute();
$result = $query->fetch(PDO::FETCH_ASSOC);

if ($result) {
    // Retourner le statut correspondant
    $status = $result['status'];
    echo json_encode(array('status' => $status));
} else {
    // Si l'ID n'est pas trouvé, retourner le statut "Inconnu"
    echo json_encode(array('status' => 'Inconnu'));
}
?>