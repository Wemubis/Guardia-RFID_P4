<?php
session_start();

// Efface toutes les variables de session
$_SESSION = array();

// Pour détruire complètement la session, efface également le cookie de session.
// Remarque : cela détruira la session, pas seulement les données de session !
if (ini_get("session.use_cookies")) {
    $params = session_get_cookie_params();
    setcookie(session_name(), '', time() - 42000,
        $params["path"], $params["domain"],
        $params["secure"], $params["httponly"]
    );
}

// Enfin, détruit la session
session_destroy();

// Redirige vers la page de connexion après la déconnexion
header("Location: login.php");
exit();
?>
