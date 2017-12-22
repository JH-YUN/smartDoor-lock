<?php
$config=array(
  "host"=>"****",
  "duser"=>"****",
  "dpw"=>"****",
  "dname"=>"****",
  "dsocket"=>"/cloudsql/YOUR_GOOGLE SQL_INSTANCE CONNECT NAME"
);
function db_init($host,$duser,$dpw,$dname,$dsocket){
 
 $conn=mysqli_connect(null,$duser,$dpw,$dname,null,$dsocket);
 
 if (mysqli_connect_errno())
  {
 	 echo mysqli_connect_error();
  }
  return $conn;
}
 ?>
