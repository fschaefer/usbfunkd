var config = {
    "paper": {
        "x": 0,
        "y": 0,
        "width": 488,
        "height": 663,
        "background": "images/background.png",
        "paths": [
        /*
            {
                "value": "M26,27.5H6c-0.829,0-1.5-0.672-1.5-1.5V6c0-0.829,0.671-1.5,1.5-1.5h20c0.828,0,1.5,0.671,1.5,1.5v20C27.5,26.828,26.828,27.5,26,27.5zM7.5,24.5h17v-17h-17V24.5z",
                "attr": {
                    "fill": "#000",
                    "stroke": "none"
                },
                "translate": {
                    "x": 10,
                    "y": 10
                }
            }
        */
        ],
        /*
        "images": [
            {
                "x": 20,
                "y": 20,
                "width": 48,
                "height": 48,
                "background": "images/cat.png"
            },
            {
                "x": 300,
                "y": 250,
                "width": 48,
                "height": 48,
                "background": "images/cat.png"
            },
            {
                "x": 80,
                "y": 90,
                "width": 48,
                "height": 48,
                "background": "images/cat.png"
            }
        ],
        "texts": [
            {
                "value": "foobar",
                "x": 100,
                "y": 100,
                "attr": {
                    "fill": "red",
                    "font-size": 30
                }
            }
        ]
    */
    },
    "devices": [
        {
            "x": 20,
            "y": 200,
            "width": 48,
            "height": 48,
            "background": "images/light_bulb_off.png",
            "click": "images/light_bulb_on.png",
            "switch": {
                "type": "pt2",
                "housecode": "7",
                "code": "B",
                "status": "off"
            }
        },
        {
            "x": 70,
            "y": 200,
            "width": 48,
            "height": 48,
            "background": "images/light_bulb_on.png",
            "click": "images/light_bulb_off.png",
            "switch": {
                "type": "pt2",
                "housecode": "7",
                "code": "B",
                "status": "on"
            }
        },
        {
            "x": 400,
            "y": 450,
            "width": 48,
            "height": 48,
            "background": "images/light_bulb_off.png",
            "click": "images/light_bulb_on.png",
            "switch": {
                "type": "pt2",
                "housecode": "7",
                "code": "C",
                "status": "off"
            }
        },
        {
            "x": 400,
            "y": 500,
            "width": 48,
            "height": 48,
            "background": "images/light_bulb_on.png",
            "click": "images/light_bulb_off.png",
            "switch": {
                "type": "pt2",
                "housecode": "7",
                "code": "C",
                "status": "on"
            }
        }
    ]
};
