const socket = io();

const posiciones = {
  PB_1: { top: 27, left: 49 },
  PB_2: { top: 74, left: 50 },
  PB_3: { top: 74, left: 31 },

  PA_1: { top: 74, left: 50 },
  PA_2: { top: 74, left: 39 },
  PA_3: { top: 74, left: 27 }
};

let estadoActual = {};
let pisoActual = "PB";

function renderAutos() {
  const container = document.getElementById("plano-container");

  Object.keys(posiciones).forEach(sensor => {
    let img = document.getElementById(sensor);

    // Crear si no existe
    if (!img) {
      img = document.createElement("img");
      img.id = sensor;
      img.className = "auto";
      container.appendChild(img);
    }

    // Ocultar si no pertenece al piso actual
    if (!sensor.startsWith(pisoActual)) {
      img.style.display = "none";
      return;
    }

    // Mostrar
    img.style.display = "block";

    // Actualizar posición y estado
    img.style.top = posiciones[sensor].top + "%";
    img.style.left = posiciones[sensor].left + "%";

    img.src = estadoActual[sensor] === 1
      ? "/static/img/A_rojo.png"
      : "/static/img/A_verde.png";
  });
}


function todosOcupadosPB() {
  return ["PB_1", "PB_2", "PB_3"]
    .every(p => estadoActual[p] === 1);
}

socket.on("estado_parqueos", data => {
  estadoActual = data;

  const planoImg = document.getElementById("plano-img");
  const titulo = document.getElementById("titulo-plano");

  if (todosOcupadosPB()) {
    pisoActual = "PA";
    planoImg.src = "/static/img/plano_PA.jpg";
    titulo.textContent = "Planta Alta";
  } else {
    pisoActual = "PB";
    planoImg.src = "/static/img/plano_PB.jpg";
    titulo.textContent = "Planta Baja";
  }

  renderAutos();
});
