# Algoritmi Híbrizi & Maximum Urgency First

## Introducere în Algoritmi Híbrizi

### Ce sunt Algoritmii Híbrizi?

Algoritmii híbrizi combină **principiile mai multor algoritmi de planificare** pentru a obține avantajele fiecăruia și a minimiza dezavantajele. Scopul este să se adapteze la **scenarii reale complexe** cu cerințe eterogene.

### De ce avem nevoie de algoritmi híbrizi?

- **RMS/DMS** sunt optime dar rigide (prioritate statică)
- **EDF/LLF** sunt optimale dar au overhead mare (prioritate dinamică)
- Sistemele reale conțin: taskuri periodice + aperiodice, deadline-uri mixte, constrângeri de resurse
- Nicio abordare pură nu se potrivește perfect tuturor scenariilor

### Principiile algoritmilor híbrizi

✓ Combină prioritate **statică și dinamică**
✓ Se adaptează la **tipul de taskuri** (periodice vs aperiodice)
✓ Balanțează între **optimalitate și overhead computațional**
✓ Lucrează cu **taskuri de prioritate mixtă** (hard real-time + soft real-time)

### Exemple de algoritmi híbrizi

- Maximum Urgency First (MUF)
- Deadline Monotonic + EDF
- Priority-based EDF
- Hierarchical scheduling

## Maximum Urgency First (MUF)

### Ce este Maximum Urgency First?

Maximum Urgency First este un algoritm **hibrid** care combină **Deadline Monotonic (DM)** pentru taskuri statice cu **Earliest Deadline First (EDF)** pentru taskuri dinamice/aperiodice. Prioritatea se bazează pe conceptul de **urgență**.

### Funcționare

1. Taskuri **periodice statice** = primesc prioritate inițială bazată pe **Deadline Monotonic** (deadline relativ mic = prioritate mare)
2. Taskuri **aperiodice/dinamice** = sunt planificate cu principiile **EDF** (deadline absolut cel mai mic = prioritate maximă)
3. Urgența unui task = combinație între deadline-ul relativ și absolut

### Definiția urgență

$$Urgency = f(Deadline_{relativ}, Deadline_{absolut}, Timp_{ramas})$$

### Avantajele MUF

✓ Optim pentru sisteme cu **mix de taskuri periodice și aperiodice**
✓ Garantează deadline-urile taskurilor periodice critice
✓ Flexibil la taskuri aperiodice neașteptate
✓ Overhead mai mic decât LLF
✓ Ușor de implementat în sisteme reale

### Cazuri de utilizare

- Sisteme de control industrial cu taskuri periodice + alerte de urgență
- Sisteme multimedia cu stream-uri regulate + procesare dinamică
- Controlere embedded cu background tasks + interrupt handling

## Implementare și comparație

### Cum funcționează MUF în practică?

MUF determină prioritatea la **fiecare preempțiune** folosind o metrică hibridă:

$$Priority_{MUF} = w_1 \cdot \frac{1}{D_i} + w_2 \cdot \frac{1}{Laxity_i} + w_3 \cdot \frac{Arrival}{T_i}$$

Unde:
- **Dᵢ** = deadline relativ
- **Laxityᵢ** = laxitate curentă
- **Arrival/Tᵢ** = raport între timp de sosire și perioadă
- **w₁, w₂, w₃** = ponderi configurabile

## Comparație: Toți algoritmii de planificare

| Algoritm | Tip | Optim | Utilizare | Taskuri aperiodice | Overhead |
|----------|-----|-------|-----------|-------------------|----------|
| **RMS** | Static | Da (periodice) | ~69% | Nu | Scăzut |
| **DMS** | Static | Da (cu deadline) | ~69% | Nu | Scăzut |
| **EDF** | Dinamic | Da (optimul) | 100% | Da | Moderat |
| **LLF** | Dinamic | Da (optimul) | 100% | Da | Ridicat |
| **MUF** | Hibrid | Pseudo-optimal | 95%+ | Da | Moderat-Ridicat |

### Avantajul MUF vs algoritmi puri

Scenario real: 5 taskuri periodice critice + 3 taskuri aperiodice aleatorii
- **RMS/DMS**: Taskurile aperiodice pot rata deadline ❌
- **EDF**: Overhead mare cu preempțiuni frecvente ⚠️
- **MUF**: Balanță automatică între stabilitate și flexibilitate ✓

## Concluzie

Algoritmii híbrizi precum MUF sunt standard în sistemele moderne de timp real, oferind balance între optimalitate teoretică și fezabilitate practică.
