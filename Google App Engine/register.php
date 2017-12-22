<?php
require_once('./config.php')
 ?>

<?php
$conn=db_init($config['host'],$config['duser'],$config['dpw'],$config['dname'],$config['dsocket']);

$nfc_id=$_GET['nfc_id'];
//$user=$_GET['user'];
//$grade=$_GET['grade'];
$sql="SELECT * FROM dl_user WHERE nfc_uid='{$nfc_id}'"; //nfc_id가 DB에 저장된 값인지 검색
$result=mysqli_query($conn,$sql);

if($result->num_rows==0){ //저장되지 않은 값이라면
  echo "clear"; //사용자 등록
  //$sql="INSERT INTO dl_user(nfc_uid,user,grade,date) VALUES('{$nfc_id}','{$user}','{$grade}',now())";
  $sql="INSERT INTO dl_user(nfc_uid,user,grade,date) VALUES('{$nfc_id}','null','null',now())";
  mysqli_query($conn,$sql);
}
else{   //저장된 값이라면
  echo "already"; //이미 등록된 사용자 정보
}


?>
