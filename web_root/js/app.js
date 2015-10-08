var $ = function (el) {
    return document.querySelectorAll(el);
}

var paper = Raphael(config.paper.x, config.paper.y, config.paper.width, config.paper.height);
paper.image(config.paper.background, config.paper.x, config.paper.y, config.paper.width, config.paper.height);

if (config.paper.paths instanceof Array) {
    config.paper.paths.forEach(function (path) {
        if (path.value) {
            var p = paper.path(path.value)
            if (path.attr) {
                p.attr(path.attr);
            }
            if (path.translate && path.translate.x && path.translate.y) {
                p.translate(path.translate.x, path.translate.y);
            }
        }
    });
}

if (config.paper.images instanceof Array) {
    config.paper.images.forEach(function (image) {
        var preload = [];
        preload[0] = new Image();
        preload[0].src = image.background;
        paper.image(image.background, image.x, image.y, image.width, image.height);
    });
}

if (config.paper.texts instanceof Array) {
    config.paper.texts.forEach(function (text) {
        paper.text(text.x, text.y, text.value).attr(text.attr);
    });
}

if (config.devices instanceof Array) {
    config.devices.forEach(function(device) {
        var preload = [];
        preload[0] = new Image();
        preload[0].src = device.background;

        var image = paper.image(device.background, device.x, device.y, device.width, device.height);

        if (device.click) {
            preload[1] = new Image();
            preload[1].src = device.click;
        }

        if (device.switch) {
            image.hover(function () {
                    this.node.style.cursor = "pointer";
                },
                function () {
                    this.node.style.cursor = "";
                });

            image.click(function () {
                    var data = [];
                    Object.keys(device.switch).forEach(function (key) {
                        data.push(":key=:value".replace(':key', key).replace(':value', device.switch[key]));
                    });

                    data = data.join('&');

                    image.node.href.baseVal = device.click;

                    var xhr = new XMLHttpRequest();
                    xhr.open("POST", "/api/v1/switch", true);
                    xhr.onreadystatechange = function (data) {
                        console.log(data);
                        image.node.href.baseVal = device.background;
                    }
                    xhr.send(data);
                });
        }
    });
}

