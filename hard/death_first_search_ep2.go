package main

import (
    "fmt"
    "os"
)

type Graph struct {
    Nodes []Node
}

type Node struct {
    Index        int
    IsGateway    bool
    Initialized  bool
    Edges        []Edge
    // Used in the search algorithm
    State        int    // 0 = undiscovered, 1 = discovered/added to queue, 2 = visited
    Parent       int    // Node which first discovered it
    Depth        int    // Distance from initial node
    PathGateways int    // Number of gateways along the path to this node
}

type Edge struct {
    Weight int
    Start *Node
    End *Node
}


func NewGraph(numNodes int) Graph {
    g := Graph{}
    g.Nodes = make([]Node, numNodes)
    return g
}

func (g *Graph) MakeNode(index int) {
    newNode := Node{Index: index, Initialized: true}
    newNode.Edges = make([]Edge, 0)
    g.Nodes[index] = newNode
}

func (g *Graph) MakeEdge(start, end *Node, weight int) {
    start.Edges = append(start.Edges, Edge{Weight: weight, Start: start, End: end})
    end.Edges = append(end.Edges, Edge{Weight: weight, Start: end, End: start})
}

func (g *Graph) RemoveEdge(start, end *Node) {
    for i, edge := range(start.Edges) {
        if edge.End.Index == end.Index {
            start.Edges = append(start.Edges[:i], start.Edges[i+1:]...)
        }
    }
    for i, edge := range(end.Edges) {
        if edge.End.Index == start.Index {
            end.Edges = append(end.Edges[:i], end.Edges[i+1:]...)
        }
    }
}

func (g *Graph) Neighbors(node Node) []*Node {
    neighbors := make([]*Node, 0)
    for i := 0; i < len(node.Edges); i++ {
        neighbors = append(neighbors, node.Edges[i].End)
    }
    return neighbors
}

func isInSlice(a int, s []int) bool {
    for _, i := range(s) {
        if a == i {
            return true
        }
    }
    return false
}

func (g *Graph) ResetMetadata() {
    for i := range(g.Nodes) {
        g.Nodes[i].State = 0
        g.Nodes[i].Depth = -1
        g.Nodes[i].Parent = -1
    }
}

// Use BFS to traverse the graph
func (g *Graph) Search(start int, end []int) (int, int) {
    queue := []*Node{&g.Nodes[start]}
    g.Nodes[start].State = 1
    g.Nodes[start].Depth = 0
    
    first, second := -1, -1
    
    var gatewayIndex int
    
    for len(queue) > 0 {
        node := queue[0]
        queue = queue[1:]
        node.State = 2
        
        fmt.Fprintf(os.Stderr, "Visiting node %d\n", node.Index)
        
        neighbors := g.Neighbors(*node)
        gatewaysConnected := 0
        
        for _, neighbor := range(neighbors) {
            //fmt.Fprintf(os.Stderr, "Checking neighbor %d\n", neighbor.Index)
            
            if isInSlice(neighbor.Index, end) {
                gatewaysConnected++
                gatewayIndex = neighbor.Index
            } else if neighbor.State == 0 {
                neighbor.State = 1
                neighbor.Parent = node.Index
                neighbor.Depth = node.Depth + 1
                queue = append(queue, neighbor)
            }
        }
        
        
        if node.Parent != -1 {
            node.PathGateways = g.Nodes[node.Parent].PathGateways + gatewaysConnected
        } else {
            node.PathGateways = gatewaysConnected
        }
        
        // If the path leading to this node contains more edges to gateways
        // than the time the agent uses to get there, we must break this link
        if node.PathGateways > node.Depth {
            return node.Index, gatewayIndex
        }
        
        // The first link found by the search
        if first == -1 && gatewaysConnected > 0 {
            first = node.Index
            second = gatewayIndex
        }
        
    }
    
    // It does not matter which link we break, 
    // just break the one closest to the agent
    return first, second
    
}

func main() {
    // N: the total number of nodes in the level, including the gateways
    // L: the number of links
    // E: the number of exit gateways
    var N, L, E int
    fmt.Scan(&N, &L, &E)
    
    g := NewGraph(N)
    gateways := make([]int, E)
    
    for i := 0; i < L; i++ {
        // N1: N1 and N2 defines a link between these nodes
        var N1, N2 int
        fmt.Scan(&N1, &N2)
        
        if !g.Nodes[N1].Initialized {
            g.MakeNode(N1)
        }
        if !g.Nodes[N2].Initialized  {
            g.MakeNode(N2)
        }
        
        g.MakeEdge(&g.Nodes[N1], &g.Nodes[N2], 1)
    }
    
    for i := 0; i < E; i++ {
        // EI: the index of a gateway node
        var EI int
        fmt.Scan(&EI)
        gateways[i] = EI
        g.Nodes[EI].IsGateway = true
    }
    
    
    for {
        // SI: The index of the node on which the Skynet agent is positioned this turn
        var SI int
        fmt.Scan(&SI)
        
        g.ResetMetadata()
        
        first, second := g.Search(SI, gateways)
        
        g.RemoveEdge(&g.Nodes[first], &g.Nodes[second])
            
        fmt.Printf("%d %d\n", first, second)
        //fmt.Fprintln(os.Stderr, "Debug messages...")
    }
}