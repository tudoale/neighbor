<?php

include ('conexao.php'); //$tabela
 
// conexão e seleção do banco de dados
$con = mysqlI_connect($host, $user, $pass, $db);
 
// executa a consulta
$sql = "SELECT * FROM $tabela ORDER BY id";
$res = mysqli_query($con, $sql);
 
// conta o número de registros
$total = mysqli_num_rows($res);
 
//echo "<p>Total de Resultados: " . $total . "</p>";
//$datahora= date("d/m/Y h:i:s");
 
// loop pelos registros
if ($f = mysqli_fetch_array($res))
{
    $botao_1= $f['botao_1'];
	$botao_2= $f['botao_2'];
	$status_bt1= $f['status_bt1'];
	$status_bt2= $f['status_bt2'];
	$tempo = $f['tempo'];
	$comando = $f['comando'];
	
	echo $botao_1.$botao_2.$comando.$tempo ;
}
 
// fecha a conexão
mysqli_close($con);
?>