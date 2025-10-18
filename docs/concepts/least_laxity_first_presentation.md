# Least Laxity First (LLF)

## Conceptul de bază

Least Laxity First este un algoritm de planificare **dinamic** pentru sisteme de timp real care se bazează pe conceptul de **laxitate (slack time)**. La fiecare moment, se execută taskul cu cea mai **mică marjă de timp disponibilă**.

### Definiția laxității

$$Laxity = Deadline_{absolut} - Timp_{curent} - Timp_{ramas}$$

Sau mai simplu: **Laxity = Timp până la deadline - Timp de execuție rămas**

### Regula fundamentală

- Taskuri cu **laxitate mai mică** → **prioritate mai înaltă**
- Taskuri cu **laxitate mai mare** → **prioritate mai joasă**
- Prioritatea se **recalculează dinamic** la fiecare timp unitar

**Exemplu:**
- Task A: deadline 50ms, timp rămas 20ms → Laxity = 30ms
- Task B: deadline 60ms, timp rămas 35ms → Laxity = 25ms
- **Task B are prioritate (laxitate mai mică)**, riscă mai mult să rateze deadline-ul.

## Avantaje și caracteristici

### De ce este util LLF?

✓ **Algoritm optim** - gestionează situații critice mai bine decât EDF
✓ **Awareness la marjă de timp** - prioritizează taskurile în situații critice
✓ **Dinamic** - se adaptează în timp real la schimbări în sistem
✓ **Previne întârzierile** - detectează devreme taskurile care riscă deadline-ul
✓ **Flexibil** - funcționează cu taskuri periodice și aperiodice

### Diferență LLF vs EDF

- **EDF** = ține cont doar de deadline-ul absolut
- **LLF** = ține cont de deadline și de **cât timp mai este de lucrat**
- LLF poate reschedula mai des și ajusta prioritățile mai fin

### Proprietăți importante

- LLF este **optimal** - dacă un sistem este planificabil, LLF îl va planifica
- LLF este mai "agresiv" decât EDF în reatribuirea priorităților
- Preempțiune: se schimbă taskul când un altul devine mai critic (laxitate mai mică)
- Overhead mai mare: necesită recalculare constantă a laxității

## Testul de planificabilitate

Pentru taskuri periodice, testul de utilizare este **identic cu EDF**:

$$U = \sum_{i=1}^{n} \frac{C_i}{T_i} \leq 1$$

Unde:
- **Cᵢ** = timp de execuție al taskului i
- **Tᵢ** = perioada taskului i
- **n** = numărul de taskuri

**De ce?** LLF este optimal, la fel ca EDF, deci condiția de planificabilitate este aceeași.

## Comparație cu alți algoritmi

| Algoritm | Prioritate | Dinamic | Utilizare | Overhead |
|----------|-----------|--------|-----------|----------|
| **RMS** | Statică (perioadă) | Nu | ~69% | Scăzut |
| **DMS** | Statică (deadline) | Nu | ~69% | Scăzut |
| **EDF** | Dinamică (deadline abs.) | Da | 100% | Moderat |
| **LLF** | Dinamică (laxitate) | Da | 100% | Ridicat |

### Avantajul LLF în practică

Scenario: 2 taskuri cu aceeași deadline dar laxități diferite
- **EDF** = alege bazat pe deadline (pot fi la egalitate)
- **LLF** = alege pe baza laxității (selectează mai precis taskul critic)

**Dezavantaj:** LLF necesită mai multă putere de calcul pentru recalcularea constantă a laxității, deci este mai rar folosit în practică decât EDF.
