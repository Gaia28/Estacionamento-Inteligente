<?php 
    $username = $_SESSION['username'] ?? 'Visitante'; 
    $route = $_GET['route'];
    
    if ($route == 'monitor'){
        $link = '?route=logout'; 
        $text = 'Sair';
    } else {
        $link = '?route=monitor';
        $text = 'Voltar ao Monitoramento';
    }
?>
 
<header class="main-header">
    <div class="system-title">
        Monitoramento de Vagas
    </div>
    
    <div class="user-info">
        <span>Olá, <strong><?php echo htmlspecialchars($username); ?></strong></span>

        <?php 
        $is_admin = (isset($username) && $username === 'admin');
        
        if ($is_admin && $route === 'monitor'): 
        ?>
            <a href="?route=manage_users" class="admin-link">Gerenciar Usuários</a>
        <?php endif; ?>

        <a href="<?php echo htmlspecialchars($link); ?>" class="logout-link"><?php echo htmlspecialchars($text); ?></a>
    </div>
</header>