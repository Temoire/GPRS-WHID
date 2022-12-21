$c=(gwmi Win32_SerialPort|Where-Object{$_.PNPDeviceID -match "ID_2341&PID_804D"}).DeviceID;# récupération du port série ou est branché le whid
$po=new-Object System.IO.Ports.SerialPort $c,9600,None,8,one;$po.open(); #ouverture du port


##Server
$port=2050
$IPEndPoint=New-Object System.Net.IPEndPoint([IPAddress]::Any,$port)
#The TcpListener class provides simple methods that listen for and accept incoming connection requests in blocking synchronous mode. You can use either a TcpClient or a Socket to connect with a TcpListener. Create a TcpListener using an IPEndPoint, a Local IP address and port number, or just a port number. Specify Any for the local IP address and 0 for the local port number if you want the underlying service provider to assign those values for you. If you choose to do this, you can use the LocalEndpoint property to identify the assigned information, after the socket has connected.
$TcpListener=New-Object System.Net.Sockets.TcpListener $IPEndPoint
#Use the Start method to begin listening for incoming connection requests. Start will queue incoming connections until you either call the Stop method or it has queued MaxConnections. Use either AcceptSocket or AcceptTcpClient to pull a connection from the incoming connection request queue. These two methods will block. If you want to avoid blocking, you can use the Pending method first to determine if connection requests are available in the queue.
$TcpListener.Start()

$AcceptTcpClient=$TcpListener.AcceptTcpClient()


$GetStream=$AcceptTcpClient.GetStream()
$StreamReader=New-Object System.IO.StreamReader $GetStream
$StreamWriter = New-Object System.IO.StreamWriter $GetStream
While (1)
{

    $po.WriteLine($StreamReader.ReadLine())
    $StreamWriter.WriteLine($po.ReadLine())

}

#$StreamReader.ReadLine()

#$StreamReader.Dispose()
#$StreamWriter.Dispose()
#$GetStream.Dispose()
#$AcceptTcpClient.Dispose()
#Call the Stop method to close the TcpListener.
#$TcpListener.Stop()