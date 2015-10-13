function ID(str)
{
    return document.getElementById(str);
}

function AJAX()
{
    if(typeof window.ActiveXObject != 'undefined')      return (new ActiveXObject("Microsoft.XMLHTTP"));
    else                                                return (new XMLHttpRequest());
}

function DEBUG()
{
    var tmp     = "";
    for( var i = 0 ; i < arguments.length ; i++ )
    {
	if( i ) tmp += ", ";
	tmp += arguments[i];
    }

    alert(tmp);
}

