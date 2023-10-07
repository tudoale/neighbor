<?php
// Arquivo responsável por atualizar status dos botões
// Esse código é compatível com PHP 8.1
// PHPMYADMIN 5.2.0
include ('conexao.php'); //$tabela
 
// conexão e seleção do banco de dados
$con = mysqlI_connect($host, $user, $pass, $db);
$id=1;
$tempo = $_GET['t'];
$botao = $_GET['b'];
$acionamento = $_GET['a'];
$status_botao = $_GET['s'];
$botao_r = "botao_".$botao;
$status_b = "status_bt".$status_botao;
//echo $botao_r." = ".$acionamento;

if($botao >= 1 & $botao <= 2) {
	$results = "UPDATE $tabela SET $botao_r = '$acionamento' WHERE id='$id'";
	$resultado_configs = mysqli_query($con, $results);
}

if($status_botao >= 1 & $status_botao <= 2) {
	$results = "UPDATE $tabela SET $status_b  = '$acionamento' WHERE id='$id'";
	$resultado_configs = mysqli_query($con, $results);
}

if($tempo > 0 & $tempo < 101) {
	$results = "UPDATE $tabela SET tempo  = '$tempo' WHERE id='$id'";
	$resultado_configs = mysqli_query($con, $results);
}

 // fecha a conexão
mysqli_close($con);
?>
