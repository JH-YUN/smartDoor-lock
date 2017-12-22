<?php
require_once('./config.php')
 ?>

<?php
$conn=db_init($config['host'],$config['duser'],$config['dpw'],$config['dname'],$config['dsocket']);//디비접속

$nfc_id=$_GET['nfc_id'];
$sql="SELECT * FROM dl_user WHERE nfc_uid='{$nfc_id}'"; //nfc_id가 DB에 저장된 값인지 검색
$result=mysqli_query($conn,$sql);
$row=mysqli_fetch_assoc($result);
if($result->num_rows!=0){ //저장된 값이라면
  if($row['grade']=='null' && $row['user']=='null'){//저장된 값이지만 user혹은 grade가 null일땐 문이 열리지 않음->미인증
    echo "noregister";
  }
  else{
  echo "ooopen"; //문열림
  $sql="INSERT INTO dl_record(nfc_uid,date) VALUES('{$nfc_id}',now())";
  mysqli_query($conn,$sql);
  }
}
else{   //저장되지 않은 값이라면
  echo "cloose"; //안열림
}

?>
