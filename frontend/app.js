const canvas = document.getElementById('radarCanvas');
const ctx = canvas.getContext('2d');
const log = document.getElementById('systemLog');

// Placeholder for simulated data until WebSocket is connected
let fleet = [];

// WebSocket Setup
const socket = new WebSocket('ws://localhost:8080');

socket.onopen = () => {
    addLog("WebSocket connected to Aegis Command.");
};

socket.onmessage = (event) => {
    // Expecting JSON telemetry from Boost.Asio backend
    fleet = JSON.parse(event.data);
    drawRadar();
};

document.getElementById('dispatchBtn').addEventListener('click', () => {
    const type = document.getElementById('planeType').value;
    const tx = document.getElementById('targetX').value;
    const ty = document.getElementById('targetY').value;
    
    const command = { action: 'DISPATCH', type, tx, ty };
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify(command));
        addLog(`Dispatched ${type} to [${tx}, ${ty}]`);
    } else {
        addLog("Error: Backend offline. Cannot dispatch.");
    }
});

function addLog(msg) {
    const p = document.createElement('p');
    p.textContent = `> ${msg}`;
    log.appendChild(p);
    log.scrollTop = log.scrollHeight;
}

function drawRadar() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Draw Grid
    ctx.strokeStyle = '#cddafd';
    ctx.lineWidth = 1;
    for(let i=0; i<canvas.width; i+=50) {
        ctx.beginPath(); ctx.moveTo(i, 0); ctx.lineTo(i, canvas.height); ctx.stroke();
        ctx.beginPath(); ctx.moveTo(0, i); ctx.lineTo(canvas.width, i); ctx.stroke();
    }

    // Draw Fleet
    fleet.forEach(plane => {
        if(!plane.active) return;
        
        ctx.beginPath();
        ctx.arc(plane.x, plane.y, 6, 0, Math.PI * 2);
        ctx.fillStyle = plane.type === 'FIGHTER' ? '#81b29a' : '#e07a5f';
        ctx.fill();
        
        // Draw ID & Fuel
        ctx.fillStyle = '#3d405b';
        ctx.font = '10px Arial';
        ctx.fillText(`${plane.id} (F:${Math.floor(plane.fuel)})`, plane.x + 10, plane.y + 10);
    });
}

// Initial draw
drawRadar();