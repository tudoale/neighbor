<?php
include ('conexao.php'); //$tabela;
 
// conexão e seleção do banco de dados
$con = mysqlI_connect($host, $user, $pass, $db);
 
// executa a consulta
$sql = "SELECT * FROM $tabela ORDER BY id";
$res = mysqli_query($con, $sql);
 
// conta o número de registros
$total = mysqli_num_rows($res);
 
//echo "<p>Total de Resultados: " . $total . "</p>";
$datahora= date("d/m/Y h:i:s");
 
// loop pelos registros
if ($f = mysqli_fetch_array($res))
{
     
         $botao_1= $f['botao_1'];
		 $botao_2= $f['botao_2'];
		 $status_bt1= $f['status_bt1'];
		 $status_bt2= $f['status_bt2'];
		 $tempo = $f['tempo'];
		 
		 // Comandos recebidos 
		 if($botao_1 == 1) $Chave_1 = "ON";		 
		 if($botao_1 == 0) $Chave_1 = "OFF";
		 if($botao_2 == 1) $Chave_2 = "ON";		 
		 if($botao_2 == 0) $Chave_2 = "OFF";
		 // Comandos enviados
		 if($status_bt1 == 1) $Status_Chave_1 = "ON";		 
		 if($status_bt1 == 0) $Status_Chave_1 = "OFF";
		 if($status_bt2 == 1) $Status_Chave_2 = "ON";		 
		 if($status_bt2 == 0) $Status_Chave_2 = "OFF";

		 //$datahora = $f['datahora'];
		 echo"<div class='card'>
					<div class='card-body'>
						<div class='table-responsive'>
							<table id='example' class='table table-striped table-bordered' style='width:100%'>
								<thead>
							<tr>
								<td><center>Visual</center></td>
								<td><center>Sonoro</center></td> 
								<td><center>Status Vis.</center></td> 
								<td><center>Status Son.</center></td>
								<td><center>Time</center></td>           
							</tr>
		<tr>
			<td><center>$botao_1</center></td>
			<td><center>$botao_2</center></td>
			<td><center>$status_bt1</center></td>
			<td><center>$status_bt2</center></td> 
			<td><center>$tempo</center></td>       
    </tr> 

</div>
</div>
</div>
</table>"; 
}
 
// fecha a conexão
mysqli_close($con);
?>
