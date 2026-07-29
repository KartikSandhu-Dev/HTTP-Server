const btn = document.getElementById("btn");

if (btn) {
    btn.addEventListener("click", () => {
        document.getElementById("message").textContent =
            "JavaScript is working!";
    });
}
