function AlteraEstadoRele(){
    var estado = document.getElementById("estado").innerHTML;
    if(estado === "0"){
        document.getElementById("estado").innerHTML="1";
        document.getElementById("botao").innerHTML="<a href='/?desligar' class='botao'>Desligar Rele</a>";
    } else {
        document.getElementById("estado").innerHTML="0";
        document.getElementById("botao").innerHTML="<a href='/?ligar' class='botao'>Ligar Rele</a>";
    }
}
 