<!-- Network Status -->
var usim=true;

function onInit()
{
	document.getElementById('page_title').innerHTML = '네트워크 상태 정보';
	document.getElementById('section2_title').innerHTML = '내부 포트 정보';
	//document.getElementById('ptpaddr1').innerHTML = 'P-to-P 서버';
	document.getElementById('name2').innerHTML = '포트';
	document.getElementById('ipaddr2').innerHTML = 'IP 주소';
	document.getElementById('netmask2').innerHTML = '서브넷마스크';
	document.getElementById('macaddr2').innerHTML = 'MAC 주소';
	document.getElementById('body').hidden = false;
}

function onLoad()
{
	onInit();

	enablePageTimeout();
	
	if(typeof window.ActiveXObject != 'undefined')
	{
		xmlhttp = (new ActiveXObject("Microsoft.XMLHTTP"));
	}
	else
	{
		xmlhttp = (new XMLHttpRequest());
	}
	
	var data = "/cgi-bin/network?cmd=status";
	
	xmlhttp.open( "POST", data, true );
	xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded;charset=euc-kr");
	xmlhttp.onreadystatechange = function()
	{
		if( (xmlhttp.readyState == 4) && (xmlhttp.status == 200) )
		{
			try
			{
				interfaces = xmlhttp.responseXML.documentElement.getElementsByTagName("ETH");
				for(i = 0 ; i < interfaces.length ; i++)
				{
						ifname 	= interfaces[i].getElementsByTagName("IFNAME")[0].firstChild.nodeValue;
						ipaddr  = interfaces[i].getElementsByTagName("IPADDR")[0].firstChild.nodeValue;
						netmask = interfaces[i].getElementsByTagName("NETMASK")[0].firstChild.nodeValue;
						macaddr = interfaces[i].getElementsByTagName("MACADDR")[0].firstChild.nodeValue;
			
						var table = document.getElementById("lan");
						var rowlen = table.rows.length;
						var row = table.insertRow(-1);
						
						row.insertCell(0).innerHTML = "<td class='center'><p>" + ifname + "</p></td>";
						row.insertCell(1).innerHTML = "<td class='center'><p>" + ipaddr + "</p></td>";
						row.insertCell(2).innerHTML = "<td class='center'><p>" + netmask + "</p></td>"
						row.insertCell(3).innerHTML = "<td class='center'><p>" + macaddr + "</p></td>";
						
						for(j = 0 ; j < row.cells.length ; j++)
						{
							row.cells[j].setAttribute('class', 'center');
						}
				}
				
				interfaces = xmlhttp.responseXML.documentElement.getElementsByTagName("PTP");
				for(i = 0 ; i < interfaces.length ; i++)
				{
						ifname 	= interfaces[i].getElementsByTagName("IFNAME")[0].firstChild.nodeValue;
						ipaddr  = interfaces[i].getElementsByTagName("IPADDR")[0].firstChild.nodeValue;
						netmask = interfaces[i].getElementsByTagName("NETMASK")[0].firstChild.nodeValue;
						//peer 		= interfaces[i].getElementsByTagName("PEER")[0].firstChild.nodeValue;
				
						var table = document.getElementById("wwan");
						var rowlen = table.rows.length;
						var row = table.insertRow(-1);
						
						row.insertCell(0).innerHTML = "<td class='center'><p>" + ifname + "</p></td>";
						row.insertCell(1).innerHTML = "<td class='center'><p>" + ipaddr + "</p></td>";
						row.insertCell(2).innerHTML = "<td class='center'><p>" + netmask + "</p></td>"
						//row.insertCell(3).innerHTML = "<td class='center'><p>" + peer + "</p></td>";;

						for(j = 0 ; j < row.cells.length ; j++)
						{
							row.cells[j].setAttribute('class', 'center');
						}
				}
			}
			catch(e)
			{
			}
		}
	}
	xmlhttp.send();
	
}