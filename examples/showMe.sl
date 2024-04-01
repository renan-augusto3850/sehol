drawWindow("Show me", 500, 500) {
    background-color: "gray"
}
drawText welcomeText("Clique no botao:"){
    color: "black"
}
drawButton showButton("Clique em mim") {
    width: 100,
    height: 50,
    x: 200,
    y: 200,
}
@behind
ifClickEvent(showButton) {
    drawMessageBox("Ola estou bem tambem esta?", YES_NO);
}