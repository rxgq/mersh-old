## Get Started

> [!NOTE]  
> Discontinued. I'll probably re-write in C#

<br>

> [!WARNING]  
> This project _is_ usable but also a work in progress
> 
<br>

Clone the repository:
```bash
git clone https://github.com/rxgq/mersh.git
cd mersh
```

---

<br>



Use `make` to build the tool. This generates a compiled executable.
```
make
```

<br>

Provide the path of your mermaid script to the executable.
```
./mersh.exe <path_to_mermaid_script.txt>
```

<br>

It will output the generated classes in an `./out` directory.

<br>

Original Class Definition:

 ```
class Duck{
    +string BeakColor
    +int Weight
    -bool IsAlive
    -int Age

    +Migrate()$
    +Swim()
    +string Quack()*
}
```

<br>

Generated Class:

```csharp
public class Duck {
    public string BeakColor { get; set; }

    public int Weight { get; set; }

    private bool IsAlive { get; set; }

    private int Age { get; set; }

    public static void Migrate() {

    }

    public void Swim() {

    }

    public abstract string Quack() {

    }
}
```
