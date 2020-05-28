const express = require("express");
const path = require("path");

const startStaticServer = (port = 8080) => new Promise((resolve) => {
    const app = express();
    app.use(express.static(__dirname));
    app.use(express.static(path.join(__dirname, "..", "data")));
    app.use(express.static(path.join(__dirname, "..", "node_modules", "moment")));
    app.use(express.static(path.join(__dirname, "..", "node_modules", "chart.js", "dist")));
    app.listen(port, () => {
        const url = `http://localhost:${port}`;
        resolve(url);
    });
});

startStaticServer()
    .then((url) => {
        console.log(`Server started at ${url}`);
    })
    .catch(console.error)
